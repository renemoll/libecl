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

SCENARIO("ByteReader: read data [big endian]")
{
	GIVEN("a ByteReader")
	{
		const auto data_buffer = std::array<std::byte, 21>{
			std::byte{0x12}, std::byte{0x34}, std::byte{0x56}, std::byte{0x78}, std::byte{0x98}, std::byte{0x76},
			std::byte{0x54}, std::byte{0x01}, std::byte{0x40}, std::byte{0xd1}, std::byte{0x60}, std::byte{0x42},
			std::byte{0x40}, std::byte{0x1a}, std::byte{0x2c}, std::byte{0x08}, std::byte{0x31}, std::byte{0x26},
			std::byte{0xe9}, std::byte{0x79}, std::byte{0x00}};
		auto reader = ByteReader{data_buffer, std::endian::big};

		WHEN("reading boolean values")
		{
			reader.skip(7);
			CHECK(reader.position() == 7);

			bool data1 = false;
			reader >> data1;
			CHECK(reader.position() == 8);

			reader.skip(12);
			bool data2 = false;
			reader >> data2;
			CHECK(reader.position() == 21);

			THEN("the boolean values are read correctly")
			{
				CHECK(data1);
				CHECK_FALSE(data2);
			}
		}

		WHEN("reading unsigned integers")
		{
			uint32_t data32 = 0;
			uint16_t data16 = 0;
			uint8_t data8 = 0;

			reader >> data32 >> data8 >> data16;
			CHECK(reader.position() == 7);

			THEN("the unsigned integers are read correctly")
			{
				CHECK(data32 == 0x12345678);
				CHECK(data8 == 0x98);
				CHECK(data16 == 0x7654);
			}
		}

		WHEN("reading signed integers")
		{
			int32_t data32 = 0;
			int16_t data16 = 0;
			int8_t data8 = 0;

			reader >> data32 >> data8 >> data16;
			CHECK(reader.position() == 7);

			THEN("the signed integers are read correctly")
			{
				CHECK(data32 == 0x12345678);
				CHECK(data8 == -104);
				CHECK(data16 == 0x7654);
			}
		}

		WHEN("reading single precision floating-point values")
		{
			reader.skip(8);

			float data = 0.0F;
			reader >> data;
			CHECK(reader.position() == 12);

			THEN("the single precision floating-point value is read correctly")
			{
				REQUIRE_THAT(data, Catch::Matchers::WithinULP(6.543F, 0));
			}
		}

		WHEN("reading double precision floating-point values")
		{
			reader.skip(12);

			double data = 0.0;
			reader >> data;
			CHECK(reader.position() == 20);

			THEN("the double precision floating-point value is read correctly")
			{
				REQUIRE_THAT(data, Catch::Matchers::WithinULP(6.543, 0));
			}
		}

		WHEN("seeking within the buffer")
		{
			reader.seek(10);
			CHECK(reader.position() == 10);
			uint8_t data = 0;
			reader >> data;
			CHECK(data == 0x60);

			reader.seek(1);
			CHECK(reader.position() == 1);
			reader >> data;
			CHECK(data == 0x34);

			reader.seek(data_buffer.size());
			CHECK(reader.position() == data_buffer.size());

			reader.seek(data_buffer.size() + 1);
			CHECK(reader.position() == data_buffer.size());
		}
	}
}

SCENARIO("ByteReader: read data [little endian]")
{
	GIVEN("a ByteReader")
	{
		const auto data_buffer = std::array<std::byte, 21>{
			std::byte{0x12}, std::byte{0x34}, std::byte{0x56}, std::byte{0x78}, std::byte{0x98}, std::byte{0x76},
			std::byte{0x54}, std::byte{0x01}, std::byte{0x40}, std::byte{0xd1}, std::byte{0x60}, std::byte{0x42},
			std::byte{0x40}, std::byte{0x1a}, std::byte{0x2c}, std::byte{0x08}, std::byte{0x31}, std::byte{0x26},
			std::byte{0xe9}, std::byte{0x79}, std::byte{0x00}};
		auto reader = ByteReader{data_buffer, std::endian::little};

		WHEN("reading boolean values")
		{
			reader.skip(7);
			CHECK(reader.position() == 7);

			bool data1 = false;
			reader >> data1;
			CHECK(reader.position() == 8);

			reader.skip(12);
			bool data2 = false;
			reader >> data2;
			CHECK(reader.position() == 21);

			THEN("the boolean values are read correctly")
			{
				CHECK(data1);
				CHECK_FALSE(data2);
			}
		}

		WHEN("reading unsigned integers")
		{
			uint32_t data32 = 0;
			uint16_t data16 = 0;
			uint8_t data8 = 0;

			reader >> data32 >> data8 >> data16;
			CHECK(reader.position() == 7);

			THEN("the unsigned integers are read correctly")
			{
				CHECK(data32 == 0x78563412);
				CHECK(data8 == 0x98);
				CHECK(data16 == 0x5476);
			}
		}

		WHEN("reading signed integers")
		{
			int32_t data32 = 0;
			int16_t data16 = 0;
			int8_t data8 = 0;

			reader >> data32 >> data8 >> data16;
			CHECK(reader.position() == 7);

			THEN("the signed integers are read correctly")
			{
				CHECK(data32 == 0x78563412);
				CHECK(data8 == -104);
				CHECK(data16 == 0x5476);
			}
		}

		WHEN("reading single precision floating-point values")
		{
			reader.skip(8);

			float data = 0.0F;
			reader >> data;
			CHECK(reader.position() == 12);

			THEN("the single precision floating-point value is read correctly")
			{
				REQUIRE_THAT(data, Catch::Matchers::WithinULP(56.204346F, 0));
			}
		}

		WHEN("reading double precision floating-point values")
		{
			reader.skip(5);

			double data = 0.0;
			reader >> data;
			CHECK(reader.position() == 13);

			THEN("the double precision floating-point value is read correctly")
			{
				REQUIRE_THAT(data, Catch::Matchers::WithinULP(36.75638580384195, 0));
			}
		}

		WHEN("reading past the end of the buffer")
		{
			auto window = std::span{data_buffer.begin(), 5};
			auto window_reader = ByteReader{window, std::endian::little};

			window_reader.skip(window.size());

			// Attempt to read past the end of the window
			uint8_t value{0xCA};
			window_reader >> value;

			THEN("no data is read past the window")
			{
				CHECK(value == 0xCA);
			}
		}
	}
}