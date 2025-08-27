/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_COMMUNICATION_I2C_IBUS_DRIVER_H
#define LIBECL_COMMUNICATION_I2C_IBUS_DRIVER_H

#include <cstdint>
#include <span>

namespace libecl::communication::i2c {
/**
 * \class IBusDriver
 * \brief Interface for I2C peripheral drivers.
 * \note  The implementation is platform depended.
 *
 * \a IBusDriver provdes an interface for I2C peripheral drivers to be used with \a BusScheduler.
 * \todo Reduce to simple configure and transfer functions...
 * \todo rename BusDriverInterface
 */
class IBusDriver
{
public:
	IBusDriver() = default;
	virtual ~IBusDriver() = default;
	IBusDriver(const IBusDriver&) = delete;
	IBusDriver& operator=(const IBusDriver&) = delete;

	/**
	 * \brief Read bytes from a I2C device.
	 * \note  The number of bytes read is determined by the size of the \a rx buffer.
	 *
	 * \param device_address: address of the I2C device to read from.
	 * \param[out] rx: buffer to store the received bytes.
	 */
	virtual bool read(uint8_t device_address, std::span<uint8_t> rx) = 0;

	/**
	 * \brief Write bytes to a I2C device.
	 * \note  The number of bytes written is determined by the size of the \a tx buffer.
	 *
	 * \param device_address: address of the I2C device to write to.
	 * \param[in] tx: buffer with bytes to be transmitted.
	 */
	virtual bool write(uint8_t device_address, std::span<const uint8_t> tx) = 0;

	/**
	 * \brief Perform a write operation followed by a read operation.
	 */
	virtual bool write_and_read(uint8_t device_address, std::span<const uint8_t> tx, std::span<uint8_t> rx) = 0;

protected:
	IBusDriver(IBusDriver&&) = default;
	IBusDriver& operator=(IBusDriver&&) = default;
};
}  // namespace libecl::communication::i2c

#endif
