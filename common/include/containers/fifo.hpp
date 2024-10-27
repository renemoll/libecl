#ifndef CCL_COMMON_FIFO_H
#define CCL_COMMON_FIFO_H

#include <atomic>
#include <cstddef>
#include <type_traits>
#include <utility>

/*!
 * FIFO buffer with fixed capacity.
 * Single producer, single consumer.
 *
 * When full, new entry are not accepted until space is made by popping the
 * oldest entries.
s */

namespace ecl::common {
template <typename T, std::size_t N>
class Fifo
{
public:
	using value_type = T;
	using size_type = size_t;
	using pointer = size_type;
	using const_pointer = size_type;
	using reference = std::add_lvalue_reference_t<T>;
	using const_reference = std::add_lvalue_reference_t<const T>;

	size_type capacity() const
	{
		return N;
	}

	void clear()
	{
		m_read.store(m_write.load());
	}

	bool empty() const
	{
		return !m_full && m_write.load() == m_read.load();
	}

	template <typename... Args>
	bool emplace(Args&&... args)
	{
		if (m_full) {
			return false;
		}

		const auto write = m_write.load();
		const auto read = m_read.load();
		const pointer next_write = (write + 1) % N;
		::new (&m_buffer[write].m_storage) T(std::forward<Args>(args)...);
		m_write.store(next_write);
		m_full = next_write == read;
		return true;
	}

	reference front()
	{
		assert(!empty());
		return *m_buffer[m_read.load()].get();
	}

	const_reference front() const
	{
		assert(!empty());
		return *m_buffer[m_read.load()].get();
	}

	bool pop()
	{
		const auto write = m_write.load();
		const auto read = m_read.load();

		if (!m_full && write == read) {
			return false;
		}

		const pointer next_read = (read + 1) % N;
		m_read.store(next_read);
		m_buffer[read].get()->~T();
		m_full = false;
		return true;
	}

	bool push(const value_type& value)
	{
		if (m_full) {
			return false;
		}

		const auto write = m_write.load();
		const auto read = m_read.load();
		const pointer next_write = (write + 1) % N;
		::new (&m_buffer[write].m_storage) T(value);
		m_write.store(next_write);
		m_full = next_write == read;
		return true;
	}

	bool push(value_type&& value)
	{
		if (m_full) {
			return false;
		}

		const auto write = m_write.load();
		const auto read = m_read.load();
		const pointer next_write = (write + 1) % N;
		::new (&m_buffer[write].m_storage) T(std::move(value));
		m_write.store(next_write);
		m_full = next_write == read;
		return true;
	}

	size_type size() const
	{
		if (m_full) {
			return N;
		}

		const auto write = m_write.load();
		const auto read = m_read.load();
		return write >= read ? write - read : write + N - read;
	}

	// back?
	// swap?
	// peek?
private:
	struct StorageType
	{
		T* get()
		{
			return std::launder(reinterpret_cast<T*>(m_storage));
		}

		const T* get() const
		{
			return std::launder(reinterpret_cast<const T*>(m_storage));
		}

		alignas(T) std::byte m_storage[sizeof(T)];
	};

	alignas(T) StorageType m_buffer[N];
	std::atomic<pointer> m_write{0};
	std::atomic<pointer> m_read{0};
	std::atomic<bool> m_full{false};
};
}  // namespace ecl::common

#endif
