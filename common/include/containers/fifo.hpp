#ifndef CCL_COMMON_FIFO_H
#define CCL_COMMON_FIFO_H

#include <array>
#include <atomic>
#include <cstddef>
#include <type_traits>

namespace ecl_common {
template <typename T, std::size_t N>
class Fifo
{
public:
	using container_type = std::array<T, N>;
	using value_type = container_type::value_type;
	using size_type = container_type::size_type;
	using pointer = size_type;
	using const_pointer = size_type;
	using reference = container_type::reference;
	using const_reference = container_type::const_reference;

	size_type capacity() const
	{
		return N;
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

	bool empty() const
	{
		return !m_full && m_write.load() == m_read.load();
	}

	void clear()
	{
		m_read.store(m_write.load());
	}

	reference front()
	{
		return m_buffer[m_read.load()];
	}

	const_reference front() const
	{
		return m_buffer[m_read.load()];
	}

	void push(const value_type& value)
	{
		// TODO: check available space
		const auto index = m_write.load();
		const pointer next_index = (index + 1) % N;
		m_buffer[index] = value;
		m_write.store(next_index);
		m_full = next_index == m_read.load();
	}

	void push(value_type&& value)
	{
		// TODO: check available space
		const auto index = m_write.load();
		const pointer next_index = (index + 1) % N;
		m_buffer[index] = std::move(value);
		m_write.store(next_index);
		m_full = next_index == m_read.load();
	}

	void pop()
	{
		// TODO: empty?
		const auto index = m_read.load();
		const pointer next_index = (index + 1) % N;
		m_read.store(next_index);
		m_full = false;
	}

	// operator=
	// constructors
	// back?
	// swap?
	// peek?
private:
	std::array<T, N> m_buffer;
	std::atomic<pointer> m_write{0};
	std::atomic<pointer> m_read{0};
	bool m_full = false;
};
}  // namespace ecl_common

#endif
