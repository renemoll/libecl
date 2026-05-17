/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_CONTAINERS_ALIGNED_STORAGE_H
#define LIBECL_CONTAINERS_ALIGNED_STORAGE_H

#include <cstddef>
#include <type_traits>
#include <utility>

namespace libecl::containers {

/*!
 * \brief     Aligned memory to store an arbitrary object.
 * \ttparam T Type of the object to store.
 */
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
	template <typename U = T>
		requires(std::is_nothrow_copy_constructible_v<U>)
	void store(const_reference value)
	{
		m_data = ::new (&m_storage) value_type(value);
	}

	/*!
	 * \brief Move \a value in the buffer.
	 * \pre   The buffer is empty, no object is stored in there.
	 * \post  The object is stored in the buffer.
	 */
	template <typename U = T>
		requires(std::is_nothrow_move_constructible_v<U>)
	void store(rvalue_reference value)
	{
		m_data = ::new (&m_storage) value_type(std::forward<T>(value));
	}

	/*!
	 * \brief Construct a new object in the buffer.
	 * \pre   The buffer is empty, no object is stored in there.
	 * \post  The object is stored in the buffer.
	 */
	template <typename U = T, typename... Args>
		requires(std::is_nothrow_constructible_v<U, Args...>)
	void emplace(Args&&... args)
	{
		m_data = ::new (&m_storage) value_type(std::forward<Args>(args)...);
	}

	/*!
	 * \brief Destroy the object current stored.
	 * \pre   An object is stored in the buffer.
	 * \post  The buffer is empty.
	 */
	void destroy()
	{
		if (m_data == nullptr) {
			return;
		}

		m_data->~T();
		m_data = nullptr;
	}

	/*!
	 * \return A pointer to the contents of the buffer.
	 * \note   Only valid after something has been placed inside the buffer.
	 */
	[[nodiscard]] pointer data() noexcept
	{
		return m_data;
	}

	[[nodiscard]] const_pointer data() const noexcept
	{
		return m_data;
	}

private:
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
	alignas(T) std::byte m_storage[sizeof(T)] = {};
	T* m_data = nullptr;
};
}  // namespace libecl::containers

#endif
