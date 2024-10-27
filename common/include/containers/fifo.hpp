#ifndef CCL_COMMON_FIFO_H
#define CCL_COMMON_FIFO_H

#include <array>
#include <atomic>
#include <cstddef>
#include <type_traits>

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
	using container_type = typename std::array<T, N>;
	using value_type = typename container_type::value_type;
	using size_type = typename container_type::size_type;
	using pointer = size_type;
	using const_pointer = size_type;
	using reference = typename container_type::reference;
	using const_reference = typename container_type::const_reference;

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
		assert(!empty());
		return m_buffer[m_read.load()];
	}

	const_reference front() const
	{
		assert(!empty());
		return m_buffer[m_read.load()];
	}

	bool push(const value_type& value)
	{
		if (m_full) {
			return false;
		}

		const auto write = m_write.load();
		const auto read = m_read.load();
		const pointer next_write = (write + 1) % N;
		m_buffer[write] = std::move(value);
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
		m_buffer[write] = std::move(value);
		m_write.store(next_write);
		m_full = next_write == read;
		return true;
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
		m_full = false;
		return true;
	}

	// operator=
	// back?
	// swap?
	// peek?
private:
	std::array<T, N> m_buffer;
	std::atomic<pointer> m_write{0};
	std::atomic<pointer> m_read{0};
	std::atomic<bool> m_full{false};
};
}  // namespace ecl::common

#endif
