/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_TEST_FAKE_BUS_DRIVER_H
#define LIBECL_TEST_FAKE_BUS_DRIVER_H

#include "libecl/communication/i2c/bus_driver_interface.hpp"

class FakeBusDriver : public libecl::communication::i2c::BusDriverInterface
{
public:
	bool read(uint8_t device_address, std::span<uint8_t> rx) override;
	bool write(uint8_t device_address, std::span<const uint8_t> tx) override;
	bool write_and_read(uint8_t device_address, std::span<const uint8_t> tx, std::span<uint8_t> rx) override;

	void set_rx_data(std::span<const uint8_t> rx);

	std::array<uint8_t, 256> m_tx_buffer = {};
	std::span<uint8_t> m_tx_view = {};

	std::array<uint8_t, 256> m_rx_buffer = {};
	std::span<uint8_t> m_rx_view = {};

	std::size_t m_tx_count = 0;
	std::size_t m_rx_count = 0;

	bool m_return_value = true;
};

#endif
