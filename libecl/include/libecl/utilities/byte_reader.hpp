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
	ByteReader(std::span<const std::byte> data_buffer, std::endian endianness)
		: m_data{data_buffer}
		, m_available{data_buffer}
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
	ByteReader& operator>>(T& data)
	{
		if (!can_fit<T>()) {
			return *this;
		}

		extract_value(data);
		m_available = m_available.subspan(sizeof(T));
		return *this;
	}

	/*!
	 * \brief Advance the internal offset to skip \a count bytes.
	 * \param count The number of bytes to skip (or the maximum available, whichever is smaller.)
	 */
	void skip(std::size_t count) noexcept
	{
		const auto max = std::min(count, m_available.size());
		m_available = m_available.subspan(max);
	}

private:
	std::span<const std::byte> m_data;
	std::span<const std::byte> m_available;
	std::endian m_endianness;

	//! \brief Returns the number of bytes in the data buffer.
	[[nodiscard]] std::size_t capacity() const noexcept
	{
		return m_data.size();
	}

	//! \brief Returns the number of bytes still available to extract.
	[[nodiscard]] std::size_t available_size() const noexcept
	{
		return capacity() - std::distance(m_data.begin(), m_available.begin());
	}

	//! \brief Returns true if the requested type can fit in the available bytes, false otherwise.
	template <typename T>
	[[nodiscard]] bool can_fit() const
	{
		return available_size() >= sizeof(T);
	}

	/*!
	 * \brief  Extracts a element of the requested type with the correct endianness.
	 * \tparam T     Type of the data element to extract.
	 * \param  value Reference to a variable to store the extracted element.
	 */
	template <typename T>
	void extract_value(T& value)
	{
		const auto as_bytes = reinterpret_cast<std::byte*>(&value);
		if (m_endianness == std::endian::native) {
			std::copy(m_available.begin(), m_available.begin() + sizeof(T), as_bytes);
		} else {
			std::reverse_copy(m_available.begin(), m_available.begin() + sizeof(T), as_bytes);
		}
	}
};
}  // namespace libecl::utilities

#endif
