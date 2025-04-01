/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef ECL_COMMUNICATION_BUS_SCHEDULER_H
#define ECL_COMMUNICATION_BUS_SCHEDULER_H

#include "libecl/communication/transaction.h"
#include "libecl/containers/fifo.hpp"

namespace ecl::communication {
class IBusDriver;

class BusScheduler
{
public:
	explicit BusScheduler(IBusDriver& bus);
	~BusScheduler() = default;
	BusScheduler(const BusScheduler&) = delete;
	BusScheduler& operator=(const BusScheduler&) = delete;
	BusScheduler(BusScheduler&&) = delete;
	BusScheduler& operator=(BusScheduler&&) = delete;

	bool blockingTransaction(const Transaction& transaction);
	// void scheduleTransaction(const Transaction& transaction);

	// void transferComplete();
	// void transferFailed();

private:
	// TODO: make size configurable...
	// TODO: mpsc fifo
	ecl::containers::Fifo<Transaction, 4> m_queue;
	IBusDriver* m_bus;
};
}  // namespace ecl::communication

#endif
