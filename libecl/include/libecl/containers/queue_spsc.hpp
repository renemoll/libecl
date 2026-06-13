/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_CONTAINERS_QUEUE_SPSC_H
#define LIBECL_CONTAINERS_QUEUE_SPSC_H

#include "libecl/config_options.hpp"
#include "libecl/containers/aligned_storage.hpp"

#include <array>
#include <atomic>
#include <cassert>
#include <cstddef>
#include <limits>
#include <new>
#include <type_traits>
#include <utility>

namespace libecl::containers {
/*!
 * \brief    Simple SPSC FIFO queue
 * \tparam T The type to store.
 * \tparam N Queue capacity.
 * \todo     Verify move only types
 * \todo	 Add front with Maybe type
 *
 * Single Producer, Single Consumer queue with fixed capacity.
 * - Fixed capacity.
 * - Wait-free & lock-free.
 * - Does not overwrite existing data when full.
 * - push and pop will not block, but will return false when the queue is full/empty.
 */
template <typename T, std::size_t N>
class QueueSpsc
{
	static_assert(N > 0, "Queue capacity must be greater than 0.");
	static_assert(N < std::numeric_limits<std::size_t>::max(), "Queue capacity is too large.");

public:
	using value_type = T;
	using size_type = std::size_t;
	using pointer = std::add_pointer_t<T>;
	using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;
	using reference = std::add_lvalue_reference_t<value_type>;
	using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
	using r_reference = std::add_rvalue_reference_t<value_type>;

	QueueSpsc() noexcept = default;
	QueueSpsc(QueueSpsc const&) = delete;
	QueueSpsc& operator=(QueueSpsc const&) = delete;
	QueueSpsc(QueueSpsc&&) = delete;
	QueueSpsc& operator=(QueueSpsc&&) = delete;

	~QueueSpsc() noexcept
	{
		clear();
	}

	//! \returns The maximum number of elements the queue can store.
	[[nodiscard]] constexpr size_type capacity() const noexcept
	{
		return N;
	}

	//! \returns True when the queue is empty, false otherwise.
	[[nodiscard]] bool empty() const noexcept
	{
		const auto read_index = m_read.load(std::memory_order_relaxed);
		const auto write_index = m_write.load(std::memory_order_acquire);
		return read_index == write_index;
	}

	//! \returns True when the queue is full, false otherwise.
	[[nodiscard]] bool full() const noexcept
	{
		const auto read_index = m_read.load(std::memory_order_acquire);
		const auto write_index = m_write.load(std::memory_order_relaxed);
		return next_index(write_index) == read_index;
	}

	/*!
	 * \returns A reference to the first element.
	 * \pre     Queue must not be empty.
	 * \todo    Replace with Maybe type to avoid potential UB
	 */
	// [[nodiscard]] reference front() noexcept
	// {
	// 	assert(!empty());
	// 	return *m_storage[m_read].data();
	// }

	/*!
	 * \returns A constant reference to the first element.
	 * \pre     Queue must not be empty.
	 * \todo    Replace with Maybe type to avoid potential UB
	 */
	// [[nodiscard]] const_reference front() const noexcept
	// {
	// 	assert(!empty());
	// 	return *m_storage[m_read].data();
	// }

	/*!
	 * \brief   Pushes the given \a value into an empty slot within the queue.
	 * \param   value The value to store.
	 * \returns True when \a value is accepted, false if the queue is full.
	 *
	 * \todo try_push and use try_emplace?
	 */
	bool push(const_reference value) noexcept
		requires(std::is_nothrow_copy_constructible_v<value_type>)
	{
		const size_type write_index = m_write.load(std::memory_order_relaxed);
		const size_type next_write = next_index(write_index);

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

	/*!
	 * \brief   Pushes the given \a value into an empty slot within the queue.
	 * \param   value The value to store.
	 * \returns True when \a value is accepted, false if the queue is full.
	 *
	 * \todo try_push and forward to try_emplace? is_constructible?
	 */
	bool push(r_reference value) noexcept
		requires(std::is_nothrow_move_constructible_v<value_type>)
	{
		const size_type write_index = m_write.load(std::memory_order_relaxed);
		const size_type next_write = next_index(write_index);

		if (next_write == m_read_cache) [[unlikely]] {
			m_read_cache = m_read.load(std::memory_order_acquire);
			if (next_write == m_read_cache) [[unlikely]] {
				return false;
			}
		}

		m_storage[write_index].store(std::move(value));
		m_write.store(next_write, std::memory_order_release);
		return true;
	}

	/*!
	 * \brief   Constructs a new object, in-place, in an empty slot within the queue.
	 * \param   args The arguments to the new object.
	 * \returns A pointer to the newly created object when space was available, nullptr otherwise.
	 *
	 * \todo try_emplace? and return bool?
	 */
	template <typename... Args>
		requires(std::is_nothrow_constructible_v<T, Args...>)
	pointer emplace(Args&&... args) noexcept
	{
		const size_type write_index = m_write.load(std::memory_order_relaxed);
		const size_type next_write = next_index(write_index);

		if (next_write == m_read_cache) [[unlikely]] {
			m_read_cache = m_read.load(std::memory_order_acquire);
			if (next_write == m_read_cache) [[unlikely]] {
				return nullptr;
			}
		}

		m_storage[write_index].emplace(std::forward<Args>(args)...);
		m_write.store(next_write, std::memory_order_release);
		return m_storage[write_index].data();
	}

	/*!
	 * \returns True when the last element is removed from the queue.
	 * \todo try_pop? or consume as we pop and return?
	 * \todo add alternative with optional type (move constructable).
	 */
	bool pop(value_type& value) noexcept
		requires(std::is_nothrow_move_assignable_v<value_type>)
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

		const size_type next_read = next_index(read_index);
		m_read.store(next_read, std::memory_order_release);
		return true;
	}

	/*!
	 * \brief Resets the queue, any stored objects are destroyed.
	 * \note  This operation is not thread-safe and should only be called when no other thread is accessing the queue.
	 */
	void clear()
	{
		if (!std::is_trivially_destructible_v<value_type>) {
			size_type read_index = m_read;
			size_type write_index = m_write;
			while (read_index != write_index) {
				m_storage[read_index].destroy();
				read_index = next_index(read_index);
			}
		}

		m_read = 0;
		m_write = 0;
		m_read_cache = 0;
		m_write_cache = 0;
	}

private:
	// Note: m_storage has size `N + 1` to allow differentiation between full and empty.
	std::array<AlignedStorage<value_type>, N + 1> m_storage = {};

#if defined(__cpp_lib_hardware_interference_size) && (__cpp_lib_hardware_interference_size >= 201603L)
#ifdef BOB_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winterference-size"
#endif

	constexpr static std::size_t padding_size = std::hardware_destructive_interference_size;

#ifdef BOB_COMPILER_GCC
#pragma GCC diagnostic pop
#endif
#else
	constexpr static std::size_t padding_size = 64;
#endif

	static_assert(std::atomic<size_type>::is_always_lock_free);

	alignas(padding_size) std::atomic<size_type> m_write{0};
	alignas(padding_size) std::atomic<size_type> m_read{0};
	alignas(padding_size) size_type m_write_cache{0};
	alignas(padding_size) size_type m_read_cache{0};

	[[nodiscard]] std::size_t next_index(std::size_t index) const noexcept
	{
		const auto next = index + 1;
		if (next == m_storage.size()) [[unlikely]] {
			return 0;
		}
		return next;
	}
};
}  // namespace libecl::containers

#endif
