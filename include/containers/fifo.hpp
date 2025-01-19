#ifndef ECL_CONTAINERS_FIFO_H
#define ECL_CONTAINERS_FIFO_H

#include <array>
#include <atomic>
#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

/*!
 * FIFO buffer with fixed capacity.
 *
 * This FIFO is meant for single producer/single consumer use-cases.
 */
namespace ecl::containers {
template <typename T, std::size_t N>
class Fifo
{
public:
	using value_type = T;
	using size_type = size_t;
	using pointer = std::add_pointer_t<T>;
	using const_pointer = std::add_pointer_t<const T>;
	using reference = std::add_lvalue_reference_t<T>;
	using const_reference = std::add_lvalue_reference_t<const T>;

	//! \brief Returns the maximum number of elements the queue can store.
	size_type capacity() const
	{
		return m_buffer.size() - 1;
	}

	//! \brief Returns the number of elements stored in the queue.
	size_type size() const
	{
		const size_type write_index = m_write.load();
		const size_type read_index = m_read.load();
		if (write_index >= read_index) {
			return write_index - read_index;
		} else {
			return m_buffer.size() - read_index + write_index;
		}
	}

	//! \returns True when the FIFO is empty.
	bool empty() const
	{
		return m_write.load() == m_read.load();
	}

	//! \returns A reference to the first element.
	reference front()
	{
		assert(!empty());
		return *m_buffer[m_read.load()].get();
	}

	//! \returns A constant reference to the first element.
	const_reference front() const
	{
		assert(!empty());
		return *m_buffer[m_read.load()].get();
	}

	template <typename... Args>
	pointer emplace(Args&&... args)
	{
		const auto write_index = m_write.load();
		const auto next_write = (write_index + 1) % m_buffer.size();

		if (next_write == m_read.load()) {
			return nullptr;
		}

		::new (&m_buffer[write_index].m_storage) T(std::forward<Args>(args)...);
		m_write.store(next_write);
		return m_buffer[write_index].get();
	}

	bool push(const value_type& value)
	{
		const size_type write_index = m_write.load();
		const size_type next_write = (write_index + 1) % m_buffer.size();

		if (next_write == m_read.load()) {
			return false;
		}

		::new (&m_buffer[write_index].m_storage) T(value);
		m_write.store(next_write);
		return true;
	}

	bool push(value_type&& value)
	{
		const size_type write_index = m_write.load();
		const size_type next_write = (write_index + 1) % m_buffer.size();

		if (next_write == m_read.load()) {
			return false;
		}

		::new (&m_buffer[write_index].m_storage) T(std::forward<T>(value));
		m_write.store(next_write);
		return true;
	}

	bool pop()
	{
		const size_type read_index = m_read.load();
		if (read_index == m_write.load()) {
			return false;
		}

		const size_type next_read = (read_index + 1) % m_buffer.size();
		m_read.store(next_read);
		m_buffer[read_index].get()->~T();
		return true;
	}

	void clear()
	{
		if (std::is_trivially_destructible_v<value_type>) {
			m_read.store(0);
			m_read.store(0);
		} else {
			while (!empty()) {
				pop();
			}
		}
	}

private:
	struct StorageType
	{
		pointer get()
		{
			return std::launder(reinterpret_cast<pointer>(m_storage));
		}

		const_pointer get() const
		{
			return std::launder(reinterpret_cast<const_pointer>(m_storage));
		}

		alignas(T) std::byte m_storage[sizeof(T)];
	};

	std::array<StorageType, N + 1> m_buffer;
	std::atomic<size_type> m_write{0};
	std::atomic<size_type> m_read{0};
};
}  // namespace ecl::containers

#endif
