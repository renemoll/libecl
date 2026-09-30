/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_UTILITIES_BYTE_WRITER_H
#define LIBECL_UTILITIES_BYTE_WRITER_H

#include <algorithm>
#include <bit>
#include <span>

namespace libecl::utilities {
/*!
 * \class ByteWriter
 * \brief Helper to write data into a byte based buffer.
 *
 * Given a data buffer, data can be written in-order into the empty part of the buffer. If any endian conversion is
 * needed, this will be taken care of.
 */
class ByteWriter
{
public:
	/*!
	 * \brief Constructor.
	 * \param data_buffer Buffer to write data into.
	 * \param endianness  Endianness of the data in \a data_buffer.
	 */
	constexpr ByteWriter(std::span<std::byte> data_buffer, std::endian endianness)
		: m_data{data_buffer}
		, m_endianness{endianness}
	{
	}

	/*!
	 * \brief  Write a single data element from the data buffer.
	 * \tparam T    Type of the data element to write.
	 * \param  data Element to store into the buffer.
	 * \return The current ByteWriter instance.
	 * \post   Advances the internal offset counter such that the next write operation writes into the next available
	 *         area.
	 */
	template <typename T>
	constexpr ByteWriter& operator<<(const T& data)
	{
		if (!can_fit<T>()) {
			return *this;
		}

		write_value(data);
		m_data = m_data.subspan(sizeof(T));
		return *this;
	}

	/*!
	 * \brief Advance the internal offset to skip \a count bytes.
	 * \param count The number of bytes to skip (or the maximum available, whichever is smaller.)
	 */
	constexpr void skip(std::size_t count) noexcept
	{
		const auto max = std::min(count, m_data.size());
		m_data = m_data.subspan(max);
	}

private:
	std::span<std::byte> m_data;
	std::endian m_endianness;

	//! \brief Returns the number of bytes still available to write.
	[[nodiscard]] constexpr std::size_t available_size() const noexcept
	{
		return m_data.size();
	}

	//! \brief Returns true if the requested type can fit in the available bytes, false otherwise.
	template <typename T>
	[[nodiscard]] constexpr bool can_fit() const noexcept
	{
		return available_size() >= sizeof(T);
	}

	/*!
	 * \brief  Writes an element of the requested type with the correct endianness.
	 * \tparam T     Type of the data element to write.
	 * \param  value Element to store into the buffer.
	 */
	template <typename T>
	constexpr void write_value(const T& value) noexcept
	{
		const auto* const as_bytes = reinterpret_cast<const std::byte*>(&value);
		if (m_endianness == std::endian::native) {
			std::copy(as_bytes, std::next(as_bytes, sizeof(T)), m_data.begin());
		} else {
			std::reverse_copy(as_bytes, std::next(as_bytes, sizeof(T)), m_data.begin());
		}
	}
};
}  // namespace libecl::utilities

#endif
