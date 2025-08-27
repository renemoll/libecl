/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "fake_bus_driver.h"

#include <cassert>

bool FakeBusDriver::read(uint8_t device_address, std::span<uint8_t> rx)
{
	m_rx_count++;

	const auto view = m_rx_view.subspan(0, std::min(rx.size(), m_rx_view.size()));
	std::copy(std::begin(view), std::end(view), std::begin(rx));

	return m_return_value;
}

bool FakeBusDriver::write(uint8_t device_address, std::span<const uint8_t> tx)
{
	m_tx_count++;

	assert(tx.size() <= m_tx_buffer.size());

	std::copy(std::begin(tx), std::end(tx), std::begin(m_tx_buffer));
	m_tx_view = std::span<uint8_t>(m_tx_buffer.data(), tx.size());

	return m_return_value;
}

bool FakeBusDriver::write_and_read(uint8_t device_address, std::span<const uint8_t> tx, std::span<uint8_t> rx)
{
	m_rx_count++;
	m_tx_count++;

	assert(tx.size() <= m_tx_buffer.size());

	std::copy(std::begin(tx), std::end(tx), std::begin(m_tx_buffer));
	m_tx_view = std::span<uint8_t>(m_tx_buffer.data(), tx.size());

	const auto view = m_rx_view.subspan(0, std::min(rx.size(), m_rx_view.size()));
	std::copy(std::begin(view), std::end(view), std::begin(rx));

	return m_return_value;
}

void FakeBusDriver::set_rx_data(std::span<const uint8_t> rx)
{
	assert(rx.size() <= m_rx_buffer.size());

	std::copy(std::begin(rx), std::end(rx), std::begin(m_rx_buffer));
	m_rx_view = std::span<uint8_t>(m_rx_buffer.data(), rx.size());
}
