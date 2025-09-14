/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_COMMUNICATION_I2C_CHANNEL_H
#define LIBECL_COMMUNICATION_I2C_CHANNEL_H

#include <cstdint>
#include <span>

namespace libecl::communication::i2c {
class BusScheduler;

/*!
 * \class Channel
 * \brief Provides a communication channel over I2C to a specific device.
 *
 * Instances of this class provide a communication channel to a specific device over on I2C bus.
 * For example, device drivers can use a \a Channel to communicate with a physical device.
 *
 * \todo 10 bit address?
 * \todo 16 bit registers?
 * \todo schedule functions
 * \todo on completion callbacks
 */
class Channel
{
public:
	Channel(BusScheduler& scheduler, uint8_t device_address);

	[[nodiscard]] bool read(std::span<uint8_t> rx);
	[[nodiscard]] bool write(std::span<const uint8_t> tx);

	[[nodiscard]] bool read_memory(uint8_t address, std::span<uint8_t> rx);
	[[nodiscard]] bool write_memory(uint8_t address, std::span<uint8_t> tx);

private:
	BusScheduler* m_scheduler;
	uint8_t m_device_address;
};

}  // namespace libecl::communication::i2c

#endif
