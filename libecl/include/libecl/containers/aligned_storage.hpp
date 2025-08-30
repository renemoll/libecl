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
#include <new>
#include <type_traits>
#include <utility>

namespace libecl::containers {

/*!
 * \brief     Aligned memory to store objects.
 * \ttparam T Type of the object to store.
 *
 * Implemented according to [P1413R2](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1413r2.pdf).
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
	using r_reference = std::add_rvalue_reference_t<value_type>;

	/*!
	 * \brief Store a copy of \a value in the buffer.
	 * \pre   The buffer is empty, no object is stored in there.
	 * \post  The object is stored in the buffer.
	 */
	void store(const_reference value)
	{
		::new (&m_storage) value_type(value);
	}

	/*!
	 * \brief Move \a value in the buffer.
	 * \pre   The buffer is empty, no object is stored in there.
	 * \post  The object is stored in the buffer.
	 */
	void store(r_reference value)
	{
		::new (&m_storage) value_type(std::forward<T>(value));
	}

	/*!
	 * \brief Construct a new object in the buffer.
	 * \pre   The buffer is empty, no object is stored in there.
	 * \post  The object is stored in the buffer.
	 */
	template <typename... Args>
	void emplace(Args&&... args)
	{
		::new (&m_storage) value_type(std::forward<Args>(args)...);
	}

	/*!
	 * \brief Destroy the object current stored.
	 * \pre   An object is stored in the buffer.
	 * \post  The buffer is empty.
	 */
	void destroy()
	{
		data()->~T();
	}

	/*!
	 * \return A pointer to the contents of the buffer.
	 * \note   Only valid after something has been placed inside the buffer.
	 */
	[[nodiscard]] pointer data()
	{
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
		return std::launder(reinterpret_cast<pointer>(&m_storage));
	}

	[[nodiscard]] const_pointer data() const
	{
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
		return std::launder(reinterpret_cast<const_pointer>(&m_storage));
	}

private:
	// NOLINTNEXTLINE(hicpp-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays, modernize-avoid-c-arrays)
	alignas(T) std::byte m_storage[sizeof(T)];
};
}  // namespace libecl::containers

#endif
