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

#include <cstdint>
#include <span>

namespace ecl::communication {

/*
 * For STM32:
 * define USE_HAL_SPI_REGISTER_CALLBACKS
 * - HAL_SPI_RegisterCallback(HAL_SPI_TX_COMPLETE_CB_ID, ...)
 * - HAL_SPI_RegisterCallback(HAL_SPI_RX_COMPLETE_CB_ID, ...)
 * - HAL_SPI_RegisterCallback(HAL_SPI_TX_RX_COMPLETE_CB_ID, ...)
 *
 * - read/write/writeAndRead
 *   spin until __HAL_SPI_GET_FLAG(SPI_FLAG_BSY) is false
 *   set correct callback
 *   then call HAL_SPI_TransmitReceive_DMA
 */
class BusDriverInterface
{
public:
	virtual bool read(std::span<uint8_t> rx) = 0;
	virtual bool write(std::span<const uint8_t> tx) = 0;
	virtual bool readAndWrite(std::span<const uint8_t> tx, std::span<uint8_t> rx) = 0;

protected:
	virtual ~BusDriverInterface() = default;
};

class BusScheduler
{
public:
	explicit BusScheduler(BusDriverInterface& bus);
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
	BusDriverInterface* m_bus;
};
}  // namespace ecl::communication

#endif
