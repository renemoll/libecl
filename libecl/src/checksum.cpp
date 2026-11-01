/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2026 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/utilities/checksum.hpp"

namespace libecl::utilities {

/*!
 * \brief Calculate the CRC-16 checksum of the given data using the CCITT polynomial.
 * \param data The input data to calculate the checksum for.
 * \return The computed CRC-16 checksum.
 *
 * Details:
 * - Polynomial: 0x1021 (CRC-16-CCITT-FALSE)
 * - Initial value: 0xFFFF
 * - Input reflected: false
 * - Output reflected: false
 * - Final XOR value: 0x0000
 */
uint16_t crc16(std::span<const std::byte> data)
{
	// CRC-16-CCITT-FALSE polynomial
	static constexpr uint16_t polynomial = 0x1021U;

	uint16_t crc = 0x1D0F;
	for (auto byte : data) {
		crc ^= static_cast<uint16_t>(static_cast<uint16_t>(byte) << 8U);
		for (int i = 0; i < 8; ++i) {
			if ((crc & 0x8000U) != 0) {
				crc = static_cast<uint16_t>(crc << 1U) ^ polynomial;
			} else {
				crc = static_cast<uint16_t>(crc << 1U);
			}
		}
	}
	return crc;
}

/*!
 * \brief Calculate the CRC-32 checksum of the given data using the Ethernet polynomial.
 * \param data The input data to calculate the checksum for.
 * \return The computed CRC-32 checksum.
 *
 * Details:
 * - Polynomial: 0xEDB88320 (Ethernet CRC32, reversed)
 * - Initial value: 0xFFFFFFFF
 * - Input reflected: true
 * - Output reflected: true
 * - Final XOR value: 0xFFFFFFFF
 */
uint32_t crc32(std::span<const std::byte> data)
{
	// Ethernet CRC32 polynomial (reversed)
	static constexpr uint32_t polynomial = 0xEDB88320U;

	uint32_t crc = 0xFFFFFFFFU;
	for (auto byte : data) {
		crc ^= static_cast<uint32_t>(byte) << 0U;
		for (int i = 0; i < 8; ++i) {
			if ((crc & 1U) != 0) {
				crc = crc >> 1U ^ polynomial;
			} else {
				crc >>= 1U;
			}
		}
	}

	return crc ^ 0xFFFFFFFFU;
}

}  // namespace libecl::utilities