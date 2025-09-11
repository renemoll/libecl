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

#include "libecl/communication/i2c/transaction.hpp"
#include "libecl/containers/queue.hpp"

#include <atomic>

namespace libecl::communication::i2c {
class BusDriverInterface;

/*!
 * \brief
 * \todo make queue size configurable...
 */
class BusScheduler
{
public:
	explicit BusScheduler(BusDriverInterface& bus);
	~BusScheduler() = default;
	BusScheduler(const BusScheduler&) = delete;
	BusScheduler& operator=(const BusScheduler&) = delete;
	BusScheduler(BusScheduler&&) = delete;
	BusScheduler& operator=(BusScheduler&&) = delete;

	bool blocking_transaction(const Transaction& transaction);
	void schedule_transaction(const Transaction& transaction);

	void on_transfer_complete();
	void on_transfer_error();

private:
	containers::Queue<Transaction, 10> m_queue;
	BusDriverInterface* m_bus;

	std::atomic<bool> m_busy;
};
}  // namespace libecl::communication::i2c

#endif
