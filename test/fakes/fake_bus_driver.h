/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef ECL_TEST_FAKE_BUS_DRIVER_H
#define ECL_TEST_FAKE_BUS_DRIVER_H

#include "libecl/communication/bus_scheduler.h"

class FakeBusDriver : public ecl::communication::IBusDriver
{
public:
	bool read(std::span<uint8_t> rx) override;
	bool write(std::span<const uint8_t> tx) override;
	bool readAndWrite(std::span<const uint8_t> tx, std::span<uint8_t> rx) override;

	void setRxData(std::span<const uint8_t> rx);

	std::array<uint8_t, 256> m_txBuffer = {};
	std::span<uint8_t> m_txView = {};

	std::array<uint8_t, 256> m_rxBuffer = {};
	std::span<uint8_t> m_rxView = {};

	std::size_t m_txCount = 0;
	std::size_t m_rxCount = 0;

	bool m_returnValue = true;
};

#endif
