/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "fake_bus_driver.h"

bool FakeBusDriver::read(std::span<uint8_t> rx)
{
	m_rxCount++;

	const auto view = m_rxView.subspan(0, std::min(rx.size(), m_rxView.size()));
	std::copy(std::begin(view), std::end(view), std::begin(rx));

	return m_returnValue;
}

bool FakeBusDriver::write(std::span<const uint8_t> tx)
{
	m_txCount++;

	assert(tx.size() <= m_txBuffer.size());

	std::copy(std::begin(tx), std::end(tx), std::begin(m_txBuffer));
	m_txView = std::span<uint8_t>(m_txBuffer.data(), tx.size());

	return m_returnValue;
}

bool FakeBusDriver::readAndWrite(std::span<const uint8_t> tx, std::span<uint8_t> rx)
{
	m_rxCount++;
	m_txCount++;

	assert(tx.size() <= m_txBuffer.size());

	std::copy(std::begin(tx), std::end(tx), std::begin(m_txBuffer));
	m_txView = std::span<uint8_t>(m_txBuffer.data(), tx.size());

	const auto view = m_rxView.subspan(0, std::min(rx.size(), m_rxView.size()));
	std::copy(std::begin(view), std::end(view), std::begin(rx));

	return m_returnValue;
}

void FakeBusDriver::setRxData(std::span<const uint8_t> rx)
{
	assert(rx.size() <= m_rxBuffer.size());

	std::copy(std::begin(rx), std::end(rx), std::begin(m_rxBuffer));
	m_rxView = std::span<uint8_t>(m_rxBuffer.data(), rx.size());
}
