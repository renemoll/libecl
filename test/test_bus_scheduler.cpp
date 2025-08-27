/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/communication/i2c/bus_scheduler.hpp"

#include "fakes/fake_bus_driver.h"
#include "libecl/communication/i2c/transaction.hpp"

#include <catch2/catch_test_macros.hpp>
#include <type_traits>

using namespace libecl::communication::i2c;

// static_assert(std::is_default_constructible_v<BusScheduler>);
// static_assert(std::is_nothrow_constructible_v<BusScheduler>);

// static_assert(std::is_default_constructible_v<BusScheduler>);
// static_assert(std::is_nothrow_default_constructible_v<BusScheduler>);

static_assert(!std::is_copy_constructible_v<BusScheduler>);
static_assert(!std::is_copy_assignable_v<BusScheduler>);

static_assert(!std::is_move_constructible_v<BusScheduler>);
static_assert(!std::is_nothrow_move_constructible_v<BusScheduler>);

static_assert(!std::is_move_assignable_v<BusScheduler>);
static_assert(!std::is_nothrow_move_assignable_v<BusScheduler>);

SCENARIO("BusScheduler: initialization")
{
	WHEN("constructing a BusScheduler")
	{
		FakeBusDriver bus{};
		BusScheduler dut{bus};

		THEN("no bus activity is triggered")
		{
			REQUIRE(bus.m_tx_count == 0);
			REQUIRE(bus.m_rx_count == 0);
		}
	}
}

SCENARIO("BusScheduler: blocking read")
{
	GIVEN("an initialized BusScheduler")
	{
		FakeBusDriver bus{};
		BusScheduler dut{bus};

		WHEN("a blocking read is executed successfully")
		{
			static constexpr std::array<uint8_t, 4> rxReference = {0xFF, 0xFF, 0xA5, 0x4B};
			bus.set_rx_data(std::span{rxReference});

			std::array<uint8_t, 4> rxData = {0x00, 0x00, 0x00, 0x00};
			const auto transaction = Transaction{0xAB, Transaction::Type::Read, {}, rxData};
			REQUIRE(dut.blocking_transaction(transaction));

			THEN("RX data is filled with response data")
			{
				REQUIRE(memcmp(rxData.data(), rxReference.data(), rxReference.size()) == 0);
			}
			THEN("only one read is triggered")
			{
				REQUIRE(bus.m_tx_count == 0);
				REQUIRE(bus.m_rx_count == 1);
			}
		}

		WHEN("a blocking read fails")
		{
			bus.m_return_value = false;

			static constexpr std::array<uint8_t, 4> rxReference = {0xFF, 0xFF, 0xA5, 0x4B};
			bus.set_rx_data(std::span{rxReference});

			std::array<uint8_t, 4> rxData = {0x00, 0x00, 0x00, 0x00};
			const auto transaction = Transaction{0xA5, Transaction::Type::Read, {}, rxData};
			const auto result = dut.blocking_transaction(transaction);

			THEN("the transaction fails")
			{
				REQUIRE_FALSE(result);
			}
		}
	}
}

SCENARIO("BusScheduler: blocking write")
{
	GIVEN("an initialized BusScheduler")
	{
		FakeBusDriver bus{};
		BusScheduler dut{bus};

		WHEN("a blocking write is executed successfully")
		{
			std::array<uint8_t, 4> txData = {0xF0, 0x0E, 0xD0, 0x0A};
			const auto transaction = Transaction{0x5A, Transaction::Type::Write, txData, {}};
			REQUIRE(dut.blocking_transaction(transaction));

			THEN("TX data is send to the BusDriver")
			{
				REQUIRE(memcmp(bus.m_tx_view.data(), txData.data(), txData.size()) == 0);
			}
			THEN("only one transaction is triggered")
			{
				REQUIRE(bus.m_tx_count == 1);
				REQUIRE(bus.m_rx_count == 0);
			}
		}

		WHEN("a blocking write fails")
		{
			bus.m_return_value = false;

			std::array<uint8_t, 4> txData = {0xF0, 0x0E, 0xD0, 0x0A};
			const auto transaction = Transaction{0xAA, Transaction::Type::Write, txData, {}};
			const auto result = dut.blocking_transaction(transaction);

			THEN("the transaction fails")
			{
				REQUIRE_FALSE(result);
			}
		}
	}
}

SCENARIO("BusScheduler: blocking transaction")
{
	GIVEN("an initialized BusScheduler")
	{
		FakeBusDriver bus{};
		BusScheduler dut{bus};

		WHEN("a blocking transaction is executed successfully")
		{
			static constexpr std::array<uint8_t, 4> rxReference = {0xFF, 0xFF, 0xA5, 0x4B};
			bus.set_rx_data(std::span{rxReference});

			std::array<uint8_t, 4> rxData = {0x00, 0x00, 0x00, 0x00};
			std::array<uint8_t, 4> txData = {0xF0, 0x0E, 0xD0, 0x0A};
			const auto transaction = Transaction{0xEF, Transaction::Type::WriteRead, txData, rxData};
			REQUIRE(dut.blocking_transaction(transaction));

			THEN("RX data is filled with response data")
			{
				REQUIRE(memcmp(rxData.data(), rxReference.data(), rxReference.size()) == 0);
			}
			THEN("TX data is send to the BusDriver")
			{
				REQUIRE(memcmp(bus.m_tx_view.data(), txData.data(), txData.size()) == 0);
			}
			THEN("only one transaction is triggered")
			{
				REQUIRE(bus.m_tx_count == 1);
				REQUIRE(bus.m_rx_count == 1);
			}
		}

		WHEN("a blocking transaction fails")
		{
			bus.m_return_value = false;

			static constexpr std::array<uint8_t, 4> rxReference = {0xFF, 0xFF, 0xA5, 0x4B};
			bus.set_rx_data(std::span{rxReference});

			std::array<uint8_t, 4> rxData = {0x00, 0x00, 0x00, 0x00};
			std::array<uint8_t, 4> txData = {0xF0, 0x0E, 0xD0, 0x0A};
			const auto transaction = Transaction{0x0D, Transaction::Type::WriteRead, txData, rxData};
			const auto result = dut.blocking_transaction(transaction);

			THEN("the transaction fails")
			{
				REQUIRE_FALSE(result);
			}
		}
	}
}

// SCENARIO("BusScheduler: schedule read")
// {
// 	SKIP();
// }

// SCENARIO("BusScheduler: schedule write")
// {
// 	SKIP();
// }

// SCENARIO("BusScheduler: schedule transactions")
// {
// 	SKIP();

// 	WHEN("an initialized BusScheduler")
// 	{
// 		FakeBusDriver bus{};
// 		BusScheduler dut{bus};

// 		// TODO: given no active transaction
// 		//       - a new transactions is queued and the low-level driver is
// 		//         started.
// 		//       - function returns immediately
// 		//       - once the low-level driver finishes -> the next
// 		//       transaction is
// 		//         started
// 		//                                            -> a callback is
// 		//                                            invoked

// 		// TODO: given an active transaction
// 		//       - new transaction is queued
// 		//       - new transaction starts when all previous transactions
// 		//         completed (or timed out)
// 		//       - callback is invoked when done
// 	}
// }
