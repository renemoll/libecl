/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_COMMUNICATION_I2C_BUS_SCHEDULER_H
#define LIBECL_COMMUNICATION_I2C_BUS_SCHEDULER_H

// #include "libecl/containers/fifo.hpp"

namespace libecl::communication::i2c {
class IBusDriver;
struct Transaction;

class BusScheduler
{
public:
	explicit BusScheduler(IBusDriver& bus);
	~BusScheduler() = default;
	BusScheduler(const BusScheduler&) = delete;
	BusScheduler& operator=(const BusScheduler&) = delete;
	BusScheduler(BusScheduler&&) = delete;
	BusScheduler& operator=(BusScheduler&&) = delete;

	bool blocking_transaction(const Transaction& transaction);
	// void schedule_transaction(const Transaction& transaction);

	// void transferComplete();
	// void transferFailed();

private:
	// TODO: make size configurable...
	// ecl::containers::Fifo<Transaction, 4> m_queue;
	IBusDriver* m_bus;
};
}  // namespace libecl::communication::i2c

#endif
