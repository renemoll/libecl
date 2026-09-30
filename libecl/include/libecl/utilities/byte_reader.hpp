/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_UTILITIES_BYTE_READER_H
#define LIBECL_UTILITIES_BYTE_READER_H

#include <algorithm>
#include <bit>
#include <cstring>
#include <span>

namespace libecl::utilities {
/*!
 * \class ByteReader
 * \brief Helper to extract data from a byte based buffer.
 *
 * Given a data buffer, data can be extracted in-order from the unread part of the buffer. If any endian conversion is
 * needed, this will be taken care of.
 */
class ByteReader
{
public:
	/*!
	 * \brief Constructor.
	 * \param data_buffer Buffer to extract data from.
	 * \param endianness  Endianness of the data in \a data_buffer.
	 */
	constexpr ByteReader(std::span<const std::byte> data_buffer, std::endian endianness)
		: m_data{data_buffer}
		, m_endianness{endianness}
	{
	}

	/*!
	 * \brief  Extract a single data element from the data buffer.
	 * \tparam T    Type of the data element to extract.
	 * \param  data Reference to a variable to store the extracted element.
	 * \return The current ByteReader instance.
	 * \post   Advances the internal offset counter such that the next read operation extracts the next data element.
	 */
	template <typename T>
	constexpr ByteReader& operator>>(T& data)
	{
		if (!can_fit<T>()) {
			return *this;
		}

		extract_value(data);
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
	std::span<const std::byte> m_data;
	std::endian m_endianness;

	//! \brief Returns the number of bytes still available to extract.
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
	 * \brief  Extracts a element of the requested type with the correct endianness.
	 * \tparam T     Type of the data element to extract.
	 * \param  value Reference to a variable to store the extracted element.
	 */
	template <typename T>
	constexpr void extract_value(T& value)
	{
		if (m_endianness == std::endian::native) {
			// Note: using memcpy here to copy directly from the buffer and start the object lifetime.
			std::memcpy(&value, m_data.data(), sizeof(T));
		} else {
#if defined(__cpp_lib_byteswap)
			std::memcpy(&value, m_data.data(), sizeof(T));
			value = std::byteswap(value);
#else
			/*
			 * Note: using memcpy here to be able to reverse the byte order before calling
			 * bit_cast to properly start the object lifetime.
			 */
			std::memcpy(&value, m_data.data(), sizeof(T));
			auto value_representation = std::array<std::byte, sizeof(T)>{};
			const auto span_to_copy = m_data.subspan(0, sizeof(T));
			std::copy(span_to_copy.begin(), span_to_copy.end(), value_representation.begin());
			std::ranges::reverse(value_representation);
			value = std::bit_cast<T>(value_representation);
#endif
		}
	}
};
}  // namespace libecl::utilities

#endif
