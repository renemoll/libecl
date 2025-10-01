/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/utilities/byte_reader.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <array>

using namespace libecl::utilities;

SCENARIO("ByteReader: read data")
{
	GIVEN("a data buffer containing data")
	{
		const auto data_buffer = std::array<std::byte, 21>{
			std::byte{0x12}, std::byte{0x34}, std::byte{0x56}, std::byte{0x78}, std::byte{0x98}, std::byte{0x76},
			std::byte{0x54}, std::byte{0x01}, std::byte{0x40}, std::byte{0xd1}, std::byte{0x60}, std::byte{0x42},
			std::byte{0x40}, std::byte{0x1a}, std::byte{0x2c}, std::byte{0x08}, std::byte{0x31}, std::byte{0x26},
			std::byte{0xe9}, std::byte{0x79}, std::byte{0x00}};

		WHEN("using a ByteReader [big endian]")
		{
			auto reader = ByteReader{data_buffer, std::endian::big};

			THEN("can extract boolean values")
			{
				reader.skip(7);

				bool data = false;
				reader >> data;
				REQUIRE(data);

				reader.skip(12);
				reader >> data;
				REQUIRE_FALSE(data);
			}

			THEN("can extract unsigned integers")
			{
				uint32_t data32 = 0;
				uint16_t data16 = 0;
				uint8_t data8 = 0;

				reader >> data32 >> data8 >> data16;
				REQUIRE(data32 == 0x12345678);
				REQUIRE(data8 == 0x98);
				REQUIRE(data16 == 0x7654);
			}

			THEN("can extract signed integers")
			{
				int32_t data32 = 0;
				int16_t data16 = 0;
				int8_t data8 = 0;

				reader >> data32 >> data8 >> data16;
				REQUIRE(data32 == 0x12345678);
				REQUIRE(data8 == -104);
				REQUIRE(data16 == 0x7654);
			}

			THEN("can extract single precision floating-point values")
			{
				reader.skip(8);

				float data = 0.0F;
				reader >> data;
				REQUIRE_THAT(data, Catch::Matchers::WithinULP(6.543F, 0));
			}

			THEN("can extract double precision floating-point values")
			{
				reader.skip(12);

				double data = 0.0;
				reader >> data;
				REQUIRE_THAT(data, Catch::Matchers::WithinULP(6.543, 0));
			}
		}

		WHEN("using a ByteReader [little endian]")
		{
			auto reader = ByteReader{data_buffer, std::endian::little};

			THEN("can extract boolean values")
			{
				reader.skip(7);

				bool data = false;
				reader >> data;
				REQUIRE(data);

				reader.skip(12);
				reader >> data;
				REQUIRE_FALSE(data);
			}

			THEN("can extract unsigned integers")
			{
				uint32_t data32 = 0;
				uint16_t data16 = 0;
				uint8_t data8 = 0;

				reader >> data32 >> data8 >> data16;
				REQUIRE(data32 == 0x78563412);
				REQUIRE(data8 == 0x98);
				REQUIRE(data16 == 0x5476);
			}

			THEN("can extract signed integers")
			{
				int32_t data32 = 0;
				int16_t data16 = 0;
				int8_t data8 = 0;

				reader >> data32 >> data8 >> data16;
				REQUIRE(data32 == 0x78563412);
				REQUIRE(data8 == -104);
				REQUIRE(data16 == 0x5476);
			}

			THEN("can extract single precision floating-point values")
			{
				reader.skip(8);

				float data = 0.0F;
				reader >> data;
				REQUIRE_THAT(data, Catch::Matchers::WithinULP(56.204346F, 0));
			}

			THEN("can extract double precision floating-point values")
			{
				reader.skip(5);

				double data = 0.0;
				reader >> data;
				REQUIRE_THAT(data, Catch::Matchers::WithinULP(36.75638580384195, 0));
			}
		}
	}
}