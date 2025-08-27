/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/communication/i2c/bus_scheduler.hpp"

#include "libecl/communication/i2c/ibus_driver.hpp"
#include "libecl/communication/i2c/transaction.hpp"

namespace libecl::communication::i2c {
BusScheduler::BusScheduler(IBusDriver& bus)
	: /*m_queue{}, */
	m_bus{&bus}
{
}

/*!
 * \todo: timeout
 * \todo return code
 * \todo transaction can fail...
 */
bool BusScheduler::blocking_transaction(const Transaction& transaction)
{
	switch (transaction.m_type) {
		case Transaction::Type::Read: {
			return m_bus->read(transaction.m_device_address, transaction.m_rx);
		} break;

		case Transaction::Type::Write: {
			return m_bus->write(transaction.m_device_address, transaction.m_tx);
		} break;

		case Transaction::Type::WriteRead: {
			return m_bus->write_and_read(transaction.m_device_address, transaction.m_tx, transaction.m_rx);
		} break;
	}

	return true;
}

// void BusScheduler::scheduleTransaction(const Transaction& transaction)
// {
// 	(void)transaction;

// 	// queue current transaction
// 	// if not busy
// 	// - trigger the start of the next transaction
// 	// if busy
// 	// - no action
// }

// void BusScheduler::transferComplete() {}
// void BusScheduler::transferFailed() {}

}  // namespace libecl::communication::i2c