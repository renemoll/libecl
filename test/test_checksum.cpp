/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/utilities/checksum.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace libecl::utilities;

SCENARIO("crc16")
{
	GIVEN("a known data sequence")
	{
		std::array<std::byte, 9> data = {std::byte{'1'}, std::byte{'2'}, std::byte{'3'}, std::byte{'4'}, std::byte{'5'},
										 std::byte{'6'}, std::byte{'7'}, std::byte{'8'}, std::byte{'9'}};

		WHEN("calculating the CRC-16 checksum")
		{
			auto result = crc16(data);

			THEN("the result should match the expected value")
			{
				REQUIRE(result == 0x31C3);
			}
		}
	}
}

SCENARIO("crc32")
{
	GIVEN("a known data sequence")
	{
		std::array<std::byte, 9> data = {std::byte{'1'}, std::byte{'2'}, std::byte{'3'}, std::byte{'4'}, std::byte{'5'},
										 std::byte{'6'}, std::byte{'7'}, std::byte{'8'}, std::byte{'9'}};

		WHEN("calculating the CRC-32 checksum")
		{
			auto result = crc32(data);

			THEN("the result should match the expected value")
			{
				REQUIRE(result == 0xCBF43926);
			}
		}
	}
}
