/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

 #ifndef ECL_COMMUNICATION_IBUS_DRIVER_H
 #define ECL_COMMUNICATION_IBUS_DRIVER_H
 
#include <cstdint>
#include <span>

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
namespace ecl::communication {
class IBusDriver
{
public:
	virtual ~IBusDriver() = default;

	virtual bool read(std::span<uint8_t> rx) = 0;
	virtual bool write(std::span<const uint8_t> tx) = 0;
	virtual bool readAndWrite(std::span<const uint8_t> tx, std::span<uint8_t> rx) = 0;
};
}

#endif
