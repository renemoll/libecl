/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/utilities/byte_writer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>

#include <array>

using namespace libecl::utilities;

SCENARIO("ByteWriter: write data")
{
	GIVEN("an empty data buffer")
	{
		std::array<std::byte, 21> data_buffer{};

		WHEN("using a ByteWriter [big endian]")
		{
			auto writer = ByteWriter{data_buffer, std::endian::big};

			THEN("can write boolean values")
			{
				writer.skip(7);

				bool data = true;
				writer << data;
				REQUIRE(data_buffer[7] == std::byte{0x01});

				writer.skip(12);
				writer << data;
				REQUIRE(data_buffer[20] == std::byte{0x01});
			}

			THEN("can write unsigned integers")
			{
				const uint32_t data32 = 0x12345678;
				const uint16_t data16 = 0x7654;
				const uint8_t data8 = 0x98;

				writer << data32 << data8 << data16;

				const auto expected = std::array<std::byte, 7>{
					std::byte{0x12}, std::byte{0x34}, std::byte{0x56}, std::byte{0x78},
					std::byte{0x98}, std::byte{0x76}, std::byte{0x54},
				};
				REQUIRE(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}

			THEN("can write signed integers")
			{
				const int32_t data32 = 0x12345678;
				const int16_t data16 = 0x7654;
				const int8_t data8 = -104;

				writer << data32 << data8 << data16;

				const auto expected = std::array<std::byte, 7>{
					std::byte{0x12}, std::byte{0x34}, std::byte{0x56}, std::byte{0x78},
					std::byte{0x98}, std::byte{0x76}, std::byte{0x54},
				};
				REQUIRE(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}

			THEN("can write single precision floating-point values")
			{
				float data = 6.543F;
				writer << data;

				const auto expected = std::array<std::byte, 4>{
					std::byte{0x40},
					std::byte{0xd1},
					std::byte{0x60},
					std::byte{0x42},
				};
				REQUIRE(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}

			THEN("can write double precision floating-point values")
			{
				double data = 6.543;
				writer << data;

				const auto expected = std::array<std::byte, 8>{
					std::byte{0x40}, std::byte{0x1a}, std::byte{0x2c}, std::byte{0x08},
					std::byte{0x31}, std::byte{0x26}, std::byte{0xe9}, std::byte{0x79},
				};
				REQUIRE(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}
		}

		WHEN("using a ByteWriter [little endian]")
		{
			auto writer = ByteWriter{data_buffer, std::endian::little};

			THEN("can write boolean values")
			{
				writer.skip(7);

				bool data = true;
				writer << data;
				REQUIRE(data_buffer[7] == std::byte{0x01});

				writer.skip(12);
				writer << data;
				REQUIRE(data_buffer[20] == std::byte{0x01});
			}

			THEN("can write unsigned integers")
			{
				const uint32_t data32 = 0x12345678;
				const uint16_t data16 = 0x7654;
				const uint8_t data8 = 0x98;

				writer << data32 << data8 << data16;

				const auto expected = std::array<std::byte, 7>{
					std::byte{0x78}, std::byte{0x56}, std::byte{0x34}, std::byte{0x12},
					std::byte{0x98}, std::byte{0x54}, std::byte{0x76},
				};
				REQUIRE(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}

			THEN("can write signed integers")
			{
				const int32_t data32 = 0x12345678;
				const int16_t data16 = 0x7654;
				const int8_t data8 = -104;

				writer << data32 << data8 << data16;

				const auto expected = std::array<std::byte, 7>{
					std::byte{0x78}, std::byte{0x56}, std::byte{0x34}, std::byte{0x12},
					std::byte{0x98}, std::byte{0x54}, std::byte{0x76},
				};
				REQUIRE(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}

			THEN("can write single precision floating-point values")
			{
				float data = 6.543F;
				writer << data;

				const auto expected = std::array<std::byte, 4>{
					std::byte{0x42},
					std::byte{0x60},
					std::byte{0xd1},
					std::byte{0x40},
				};
				REQUIRE(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}

			THEN("can write double precision floating-point values")
			{
				double data = 6.543;
				writer << data;

				const auto expected = std::array<std::byte, 8>{
					std::byte{0x79}, std::byte{0xe9}, std::byte{0x26}, std::byte{0x31},
					std::byte{0x08}, std::byte{0x2c}, std::byte{0x1a}, std::byte{0x40},
				};
				REQUIRE(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}
		}
	}
}