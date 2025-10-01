/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_CONTAINERS_QUEUE_H
#define LIBECL_CONTAINERS_QUEUE_H

#include "libecl/containers/aligned_storage.hpp"

#include <array>
#include <atomic>
#include <cassert>
#include <type_traits>

namespace libecl::containers {
/*!
 * \brief    Simple SPSC FIFO queue
 * \tparam T The type to store.
 * \tparam N Queue capacity.
 *
 * Single Producer, Single Consumer queue with fixed capacity.
 * - Fixed capacity
 * - Wait-free & lock-free
 *
 * \todo Conversion to value_type
 * \todo noexcept?
 * \todo memory orders
 * \todo deconstructor clears the queue
 */
template <typename T, std::size_t N>
class Queue
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using pointer = std::add_pointer_t<T>;
	using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;
	using reference = std::add_lvalue_reference_t<value_type>;
	using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
	using r_reference = std::add_rvalue_reference_t<value_type>;

	//! \returns The maximum number of elements the queue can store.
	[[nodiscard]] size_type capacity() const
	{
		return N;
	}

	//! \returns True when the queue is empty, false otherwise.
	[[nodiscard]] bool empty() const
	{
		return m_write == m_read;
	}

	[[nodiscard]] bool full() const
	{
		const size_type write_index = m_write;
		const size_type next_write = (write_index + 1) % m_storage.size();

		return next_write == m_read;
	}

	/*!
	 * \returns A reference to the first element.
	 * \pre     Queue must not be empty.
	 */
	[[nodiscard]] reference front()
	{
		assert(!empty());
		return *m_storage[m_read].data();
	}

	/*!
	 * \returns A constant reference to the first element.
	 * \pre     Queue must not be empty.
	 */
	[[nodiscard]] const_reference front() const
	{
		assert(!empty());
		return *m_storage[m_read].data();
	}

	/*!
	 * \brief   Pushes the given \a value into an empty slot within the queue.
	 * \returns True when \a valve is accepted, false if the queue is full.
	 */
	bool push(const_reference value)
	{
		const size_type write_index = m_write;
		const size_type next_write = (write_index + 1) % m_storage.size();

		if (next_write == m_read) {
			return false;
		}

		m_storage[write_index].store(value);
		m_write = next_write;
		return true;
	}

	/*!
	 * \brief   Pushes the given \a value into an empty slot within the queue.
	 * \returns True when \a valve is accepted, false if the queue is full.
	 */
	bool push(r_reference value)
	{
		const size_type write_index = m_write;
		const size_type next_write = (write_index + 1) % m_storage.size();

		if (next_write == m_read) {
			return false;
		}

		m_storage[write_index].store(std::forward<value_type>(value));
		m_write = next_write;
		return true;
	}

	/*!
	 * \brief   Constructs a new object, in-place, in an empty slot within the queue.
	 * \returns A pointer to the newly created object when space was available, nullptr otherwise.
	 */
	template <typename... Args>
	pointer emplace(Args&&... args)
	{
		const size_type write_index = m_write;
		const size_type next_write = (write_index + 1) % m_storage.size();

		if (next_write == m_read) {
			return nullptr;
		}

		m_storage[write_index].emplace(std::forward<Args>(args)...);
		m_write = next_write;
		return m_storage[write_index].data();
	}

	//! \returns True when the last element is removed from the queue.
	bool pop()
	{
		const size_type read_index = m_read;
		if (read_index == m_write.load()) {
			return false;
		}

		const size_type next_read = (read_index + 1) % m_storage.size();
		m_storage[read_index].destroy();
		m_read = next_read;
		return true;
	}

	//! \brief Resets the queue, any stored objects are destroyed.
	void clear()
	{
		if (std::is_trivially_destructible_v<value_type>) {
			m_read = 0;
			m_read = 0;
		} else {
			while (!empty()) {
				pop();
			}
		}
	}

private:
	// Note: m_storage has size `N + 1` to allow differentiation between full and empty.
	std::array<AlignedStorage<value_type>, N + 1> m_storage = {};
	std::atomic<size_type> m_write{0};
	std::atomic<size_type> m_read{0};
};
}  // namespace libecl::containers

#endif
