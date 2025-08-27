/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_COMMUNICATION_I2C_TRANSACTION_H
#define LIBECL_COMMUNICATION_I2C_TRANSACTION_H

#include <cstdint>
#include <span>

namespace libecl::communication::i2c {
/**
 * \class Transaction
 * \brief Holds all data to perform a single I2C transaction.
 *
 * I2C devices generally support these operations:
 * - a single read operation of one or more bytes;
 * - a single write operation of one or more bytes;
 * - a memory mapped read, writing a register address and reading it's contents;
 * - a memory mapped write, writing a register address and writing it's contents.
 *
 * To support these operations, a transaction is defined to either:
 * - read one or more bytes;
 * - write one or more bytes;
 * - write one or two bytes followed by reading one or more bytes.
 *
 * \todo: add address
 * \todo move documentation
 */
struct Transaction
{
	enum class Type : uint8_t
	{
		Read,
		Write,
		WriteRead,
	};

	uint8_t m_device_address;
	Type m_type;
	std::span<const uint8_t> m_tx;
	std::span<uint8_t> m_rx;
};
}  // namespace libecl::communication::i2c

#endif
