/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef ECL_COMMUNICATION_IBUS_DRIVER_H
#define ECL_COMMUNICATION_IBUS_DRIVER_H

#include <cstdint>
#include <span>

namespace ecl::communication {
class IBusDriver
{
public:
	virtual ~IBusDriver() = default;

	virtual bool read(uint8_t device_address, std::span<uint8_t> rx) = 0;
	virtual bool write(uint8_t device_address, std::span<const uint8_t> tx) = 0;
	virtual bool writeAndRead(uint8_t device_address, std::span<const uint8_t> tx, std::span<uint8_t> rx) = 0;
};
}  // namespace ecl::communication

#endif
