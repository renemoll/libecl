/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/communication/bus_scheduler.h"

#include "libecl/communication/ibus_driver.h"

namespace ecl::communication {
BusScheduler::BusScheduler(IBusDriver& bus)
	: m_queue{}
	, m_bus{&bus}
{
}

/*!
 * \todo: timeout
 * \todo return code
 * \todo transaction can fail...
 */
bool BusScheduler::blockingTransaction(const Transaction& transaction)
{
	switch (transaction.m_type) {
		case Transaction::Type::Read: {
			return m_bus->read(transaction.m_rx);
		} break;

		case Transaction::Type::Write: {
			return m_bus->write(transaction.m_tx);
		} break;

		case Transaction::Type::ReadWrite: {
			return m_bus->readAndWrite(transaction.m_tx, transaction.m_rx);
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

}  // namespace ecl::communication