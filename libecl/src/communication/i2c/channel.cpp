/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/communication/i2c/channel.hpp"

#include "libecl/communication/i2c/bus_scheduler.hpp"
#include "libecl/communication/i2c/transaction.hpp"

namespace libecl::communication::i2c {
Channel::Channel(BusScheduler& scheduler, uint8_t device_address)
	: m_scheduler{&scheduler}
	, m_device_address{device_address}
{
}

bool Channel::read(std::span<uint8_t> rx)
{
	return m_scheduler->blocking_transaction(
		{.m_device_address = m_device_address, .m_type = Transaction::Type::Read, .m_tx = {}, .m_rx = rx});
}

bool Channel::write(std::span<const uint8_t> tx)
{
	return m_scheduler->blocking_transaction(
		{.m_device_address = m_device_address, .m_type = Transaction::Type::Write, .m_tx = tx, .m_rx = {}});
}

bool Channel::read_memory(const uint8_t address, std::span<uint8_t> rx)
{
	const auto tx_register = std::span<const uint8_t, 1>{&address, 1};
	return m_scheduler->blocking_transaction({.m_device_address = m_device_address,
											  .m_type = Transaction::Type::WriteRead,
											  .m_tx = tx_register,
											  .m_rx = rx});
}

bool Channel::write_memory(const uint8_t address, std::span<uint8_t> tx)
{
	const auto tx_register = std::span<const uint8_t, 1>{&address, 1};
	return write(tx_register) && write(tx);
}

}  // namespace libecl::communication::i2c
