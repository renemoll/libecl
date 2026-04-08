/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include <array>
#include <atomic>
#include <stdexcept>
#include <thread>

#include <benchmark/benchmark.h>

namespace {
template <typename T>
class AlignedStorage
{
public:
	using value_type = T;
	using pointer = std::add_pointer_t<T>;
	using const_pointer = std::add_pointer_t<const T>;
	using reference = std::add_lvalue_reference_t<value_type>;
	using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
	using rvalue_reference = std::add_rvalue_reference_t<value_type>;

	/*!
	 * \brief Store a copy of \a value in the buffer.
	 * \pre   The buffer is empty, no object is stored in there.
	 * \post  The object is stored in the buffer.
	 */
	void store(const_reference value) noexcept
	{
		::new (&m_storage) value_type(value);
	}

	/*!
	 * \brief Move \a value in the buffer.
	 * \pre   The buffer is empty, no object is stored in there.
	 * \post  The object is stored in the buffer.
	 */
	void store(rvalue_reference value) noexcept
	{
		::new (&m_storage) value_type(std::forward<T>(value));
	}

	/*!
	 * \brief Destroy the object current stored.
	 * \pre   An object is stored in the buffer.
	 * \post  The buffer is empty.
	 */
	void destroy() noexcept
	{
		data()->~T();
	}

	/*!
	 * \return A pointer to the contents of the buffer.
	 * \note   Only valid after something has been placed inside the buffer.
	 */
	[[nodiscard]] pointer data() noexcept
	{
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
		return std::launder(reinterpret_cast<pointer>(&m_storage));
	}

	[[nodiscard]] const_pointer data() const noexcept
	{
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
		return std::launder(reinterpret_cast<const_pointer>(&m_storage));
	}

private:
	// NOLINTNEXTLINE(hicpp-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays, modernize-avoid-c-arrays)
	alignas(T) std::byte m_storage[sizeof(T)];
};

void pin_thread(int cpu_id)
{
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(cpu_id, &cpuset);
	if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
		std::exit(EXIT_FAILURE);
	}
}

template <typename Q>
void BM_QueueSpsc(benchmark::State& state)
{
	const int num_iters = state.range(0);

	for (auto _ : state) {
		auto queue = Q{};

		auto consumer = std::thread([&] {
			pin_thread(2);
			for (auto i = 0; i < num_iters; ++i) {
				auto value = 0;
				while (!queue.pop(value))
					;
				if (value != i) {
					throw std::runtime_error("Values not matching");
				}
			}
		});

		pin_thread(3);

		for (auto i = 0; i < num_iters; ++i) {
			while (!queue.push(i))
				;
		}

		consumer.join();
	}

	state.SetItemsProcessed(state.iterations() * num_iters);
}
}  // namespace

/*!
 * \brief Initial implementation of a SPSC queue.
 * \tparam T The type of elements stored in the queue.
 * \tparam N The capacity of the queue.
 *
 * Notes:
 * - The queue is bounded (fixed capacity) to an arbitrary value of 'N'. There is no limitation
 *   imposed on the size `N`.
 * - Internally, the queue uses a ring buffer of `N + 1` elements, allowing it to differentiate
 *   between full and empty states.
 * - The `push` and `pop` operations are wait-free, meaning they will complete in a finite number
 *   of steps/cycles regardless of the actions of other threads.
 */
template <typename T, std::size_t N>
class QueueV1
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using pointer = std::add_pointer_t<T>;
	using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;
	using reference = std::add_lvalue_reference_t<value_type>;
	using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
	using rvalue_reference = std::add_rvalue_reference_t<value_type>;

	bool push(const_reference value) noexcept
	{
		const size_type write_index = m_write;
		const size_type next_write = (write_index + 1) % m_storage.size();

		if (next_write == m_read) {
			return false;  // queue is full
		}

		m_storage[write_index].store(value);
		m_write = next_write;
		return true;
	}

	bool pop(T& value) noexcept
	{
		const size_type read_index = m_read;
		if (read_index == m_write.load()) {
			return false;  // queue is empty
		}

		const size_type next_read = (read_index + 1) % m_storage.size();
		value = std::move(*m_storage[read_index].data());
		m_storage[read_index].destroy();
		m_read = next_read;
		return true;
	}

private:
	// Note: m_storage has size `N + 1` to allow differentiation between full and empty.
	std::array<AlignedStorage<value_type>, N + 1> m_storage = {};
	std::atomic<size_type> m_write{0};
	std::atomic<size_type> m_read{0};
};

/*!
 * Changes w.r.t V1:
 * - added memory orderings to the atomic operations.
 */
template <typename T, std::size_t N>
class QueueV2
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using pointer = std::add_pointer_t<T>;
	using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;
	using reference = std::add_lvalue_reference_t<value_type>;
	using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
	using rvalue_reference = std::add_rvalue_reference_t<value_type>;

	bool push(const_reference value) noexcept
	{
		const size_type write_index = m_write.load(std::memory_order_relaxed);
		const size_type next_write = (write_index + 1) % m_storage.size();

		if (next_write == m_read.load(std::memory_order_acquire)) {
			return false;  // queue is full
		}

		m_storage[write_index].store(value);
		m_write.store(next_write, std::memory_order_release);
		return true;
	}

	bool pop(T& value) noexcept
	{
		const size_type read_index = m_read.load(std::memory_order_relaxed);
		if (read_index == m_write.load(std::memory_order_acquire)) {
			return false;  // queue is empty
		}

		const size_type next_read = (read_index + 1) % m_storage.size();
		value = std::move(*m_storage[read_index].data());
		m_storage[read_index].destroy();
		m_read.store(next_read, std::memory_order_release);
		return true;
	}

private:
	// Note: m_storage has size `N + 1` to allow differentiation between full and empty.
	std::array<AlignedStorage<value_type>, N + 1> m_storage = {};
	std::atomic<size_type> m_write{0};
	std::atomic<size_type> m_read{0};
};

/*!
 * Changes w.r.t V1:
 * - aligned storage for the read and write indices to different cache lines to avoid false sharing.
 * - added memory orderings to the atomic operations. (QueueV2)
 */
template <typename T, std::size_t N>
class QueueV3
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using pointer = std::add_pointer_t<T>;
	using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;
	using reference = std::add_lvalue_reference_t<value_type>;
	using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
	using rvalue_reference = std::add_rvalue_reference_t<value_type>;

	bool push(const_reference value) noexcept
	{
		const size_type write_index = m_write.load(std::memory_order_relaxed);
		const size_type next_write = (write_index + 1) % m_storage.size();

		if (next_write == m_read.load(std::memory_order_acquire)) {
			return false;  // queue is full
		}

		m_storage[write_index].store(value);
		m_write.store(next_write, std::memory_order_release);
		return true;
	}

	bool pop(T& value) noexcept
	{
		const size_type read_index = m_read.load(std::memory_order_relaxed);
		if (read_index == m_write.load(std::memory_order_acquire)) {
			return false;  // queue is empty
		}

		const size_type next_read = (read_index + 1) % m_storage.size();
		value = std::move(*m_storage[read_index].data());
		m_storage[read_index].destroy();
		m_read.store(next_read, std::memory_order_release);
		return true;
	}

private:
	// Note: m_storage has size `N + 1` to allow differentiation between full and empty.
	std::array<AlignedStorage<value_type>, N + 1> m_storage = {};
	alignas(std::hardware_destructive_interference_size) std::atomic<size_type> m_write{0};
	alignas(std::hardware_destructive_interference_size) std::atomic<size_type> m_read{0};
};

/*!
 * Changes w.r.t V1:
 * - replaced modulo with a branch to wrap around the indices.
 * - aligned storage for the read and write indices to different cache lines to avoid false sharing.
 * - added memory orderings to the atomic operations. (QueueV2)
 */
template <typename T, std::size_t N>
class QueueV4
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using pointer = std::add_pointer_t<T>;
	using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;
	using reference = std::add_lvalue_reference_t<value_type>;
	using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
	using rvalue_reference = std::add_rvalue_reference_t<value_type>;

	bool push(const_reference value) noexcept
	{
		const size_type write_index = m_write.load(std::memory_order_relaxed);
		size_type next_write = (write_index + 1);
		if (next_write == m_storage.size()) {
			next_write = 0;
		}

		if (next_write == m_read.load(std::memory_order_acquire)) {
			return false;  // queue is full
		}

		m_storage[write_index].store(value);
		m_write.store(next_write, std::memory_order_release);
		return true;
	}

	bool pop(T& value) noexcept
	{
		const size_type read_index = m_read.load(std::memory_order_relaxed);
		if (read_index == m_write.load(std::memory_order_acquire)) {
			return false;  // queue is empty
		}

		value = std::move(*m_storage[read_index].data());
		m_storage[read_index].destroy();

		size_type next_read = (read_index + 1);
		if (next_read == m_storage.size()) {
			next_read = 0;
		}
		m_read.store(next_read, std::memory_order_release);
		return true;
	}

private:
	// Note: m_storage has size `N + 1` to allow differentiation between full and empty.
	std::array<AlignedStorage<value_type>, N + 1> m_storage = {};
	alignas(std::hardware_destructive_interference_size) std::atomic<size_type> m_write{0};
	alignas(std::hardware_destructive_interference_size) std::atomic<size_type> m_read{0};
};

/*!
 * Changes w.r.t V1:
 * - cache read/write indices.
 * - replaced modulo with a branch to wrap around the indices. (QueueV4)
 * - aligned storage for the read and write indices to different cache lines to avoid false sharing. (QueueV3)
 * - added memory orderings to the atomic operations. (QueueV2)
 */
template <typename T, std::size_t N>
class QueueV5
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using pointer = std::add_pointer_t<T>;
	using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;
	using reference = std::add_lvalue_reference_t<value_type>;
	using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
	using rvalue_reference = std::add_rvalue_reference_t<value_type>;

	bool push(const_reference value) noexcept
	{
		const size_type write_index = m_write.load(std::memory_order_relaxed);
		size_type next_write = (write_index + 1);
		if (next_write == m_storage.size()) {
			next_write = 0;
		}

		if (next_write == m_read_cache) {
			m_read_cache = m_read.load(std::memory_order_acquire);
			if (next_write == m_read_cache) {
				return false;
			}
		}

		m_storage[write_index].store(value);
		m_write.store(next_write, std::memory_order_release);
		return true;
	}

	bool pop(T& value) noexcept
	{
		const size_type read_index = m_read.load(std::memory_order_relaxed);
		if (read_index == m_write_cache) {
			m_write_cache = m_write.load(std::memory_order_acquire);
			if (read_index == m_write_cache) {
				return false;  // queue is empty
			}
		}

		value = std::move(*m_storage[read_index].data());
		m_storage[read_index].destroy();

		size_type next_read = (read_index + 1);
		if (next_read == m_storage.size()) {
			next_read = 0;
		}
		m_read.store(next_read, std::memory_order_release);
		return true;
	}

private:
	// Note: m_storage has size `N + 1` to allow differentiation between full and empty.
	std::array<AlignedStorage<value_type>, N + 1> m_storage = {};
	alignas(std::hardware_destructive_interference_size) std::atomic<size_type> m_write{0};
	alignas(std::hardware_destructive_interference_size) std::atomic<size_type> m_read{0};
	alignas(std::hardware_destructive_interference_size) std::atomic<size_type> m_write_cache{0};
	alignas(std::hardware_destructive_interference_size) std::atomic<size_type> m_read_cache{0};
};

/*!
 * Changes w.r.t V1:
 * - added likely/unlikely hints.
 * - cache read/write indices. (QueueV5)
 * - replaced modulo with a branch to wrap around the indices. (QueueV4)
 * - aligned storage for the read and write indices to different cache lines to avoid false sharing. (QueueV3)
 * - added memory orderings to the atomic operations. (QueueV2)
 */
template <typename T, std::size_t N>
class QueueV6
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using pointer = std::add_pointer_t<T>;
	using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;
	using reference = std::add_lvalue_reference_t<value_type>;
	using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
	using rvalue_reference = std::add_rvalue_reference_t<value_type>;

	bool push(const_reference value) noexcept
	{
		const size_type write_index = m_write.load(std::memory_order_relaxed);
		size_type next_write = (write_index + 1);
		if (next_write == m_storage.size()) [[unlikely]] {
			next_write = 0;
		}

		if (next_write == m_read_cache) [[unlikely]] {
			m_read_cache = m_read.load(std::memory_order_acquire);
			if (next_write == m_read_cache) [[unlikely]] {
				return false;
			}
		}

		m_storage[write_index].store(value);
		m_write.store(next_write, std::memory_order_release);
		return true;
	}

	bool pop(T& value) noexcept
	{
		const size_type read_index = m_read.load(std::memory_order_relaxed);
		if (read_index == m_write_cache) [[unlikely]] {
			m_write_cache = m_write.load(std::memory_order_acquire);
			if (read_index == m_write_cache) [[unlikely]] {
				return false;  // queue is empty
			}
		}

		value = std::move(*m_storage[read_index].data());
		m_storage[read_index].destroy();

		size_type next_read = (read_index + 1);
		if (next_read == m_storage.size()) [[unlikely]] {
			next_read = 0;
		}
		m_read.store(next_read, std::memory_order_release);
		return true;
	}

private:
	// Note: m_storage has size `N + 1` to allow differentiation between full and empty.
	std::array<AlignedStorage<value_type>, N + 1> m_storage = {};
	alignas(std::hardware_destructive_interference_size) std::atomic<size_type> m_write{0};
	alignas(std::hardware_destructive_interference_size) std::atomic<size_type> m_read{0};
	alignas(std::hardware_destructive_interference_size) std::atomic<size_type> m_write_cache{0};
	alignas(std::hardware_destructive_interference_size) std::atomic<size_type> m_read_cache{0};
};

BENCHMARK(BM_QueueSpsc<QueueV1<int, 100'000>>)->Arg(100'000'000);
BENCHMARK(BM_QueueSpsc<QueueV2<int, 100'000>>)->Arg(100'000'000);
BENCHMARK(BM_QueueSpsc<QueueV3<int, 100'000>>)->Arg(100'000'000);
BENCHMARK(BM_QueueSpsc<QueueV4<int, 100'000>>)->Arg(100'000'000);
BENCHMARK(BM_QueueSpsc<QueueV5<int, 100'000>>)->Arg(100'000'000);
BENCHMARK(BM_QueueSpsc<QueueV6<int, 100'000>>)->Arg(100'000'000);

/*!
 * V1: base implementation
 * V2: added memory orderings to the atomic operations.
 * V3: aligned storage for the read and write indices to different cache lines to avoid false sharing.
 * V4: replaced modulo with a branch to wrap around the indices.
 * V5: cache read/write indices.
 * V6: added likely/unlikely hints.
 *
 * Variation   | (items/s)
 * ----------- | -----------
 *  QueueV1    |  12.8153M/s
 *  QueueV2    |  60.1057M/s
 *  QueueV3    |  78.4198M/s
 *  QueueV4    |  66.4483M/s
 *  QueueV5    | 152.294M/s
 *  QueueV6    | 177.823M/s
 *
 * Todo: rebuild with other benchmark package?
 */