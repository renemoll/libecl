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

SCENARIO("ByteWriter: write data [big endian]")
{
	GIVEN("an ByteWriter")
	{
		std::array<std::byte, 21> data_buffer{};
		auto writer = ByteWriter{data_buffer, std::endian::big};

		WHEN("writing boolean values")
		{
			writer.skip(7);
			CHECK(writer.position() == 7);

			bool data = true;
			writer << data;
			CHECK(writer.position() == 8);

			writer.skip(12);
			writer << data;
			CHECK(writer.position() == 21);

			THEN("the boolean values are written correctly")
			{
				CHECK(data_buffer[7] == std::byte{0x01});
				CHECK(data_buffer[20] == std::byte{0x01});
			}
		}

		WHEN("writing unsigned integers")
		{
			const uint32_t data32 = 0x12345678;
			const uint16_t data16 = 0x7654;
			const uint8_t data8 = 0x98;

			writer << data32 << data8 << data16;
			CHECK(writer.position() == 7);

			THEN("the unsigned integers are written correctly")
			{
				const auto expected = std::array<std::byte, 7>{
					std::byte{0x12}, std::byte{0x34}, std::byte{0x56}, std::byte{0x78},
					std::byte{0x98}, std::byte{0x76}, std::byte{0x54},
				};
				CHECK(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}
		}

		WHEN("writing signed integers")
		{
			const int32_t data32 = 0x12345678;
			const int16_t data16 = 0x7654;
			const int8_t data8 = -104;

			writer << data32 << data8 << data16;
			CHECK(writer.position() == 7);

			THEN("the signed integers are written correctly")
			{
				const auto expected = std::array<std::byte, 7>{
					std::byte{0x12}, std::byte{0x34}, std::byte{0x56}, std::byte{0x78},
					std::byte{0x98}, std::byte{0x76}, std::byte{0x54},
				};
				CHECK(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}
		}

		WHEN("writing single precision floating-point values")
		{
			float data = 6.543F;
			writer << data;
			CHECK(writer.position() == 4);

			THEN("the single precision floating-point value is written correctly")
			{
				const auto expected = std::array<std::byte, 4>{
					std::byte{0x40},
					std::byte{0xd1},
					std::byte{0x60},
					std::byte{0x42},
				};
				CHECK(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}
		}

		WHEN("writing double precision floating-point values")
		{
			double data = 6.543;
			writer << data;
			CHECK(writer.position() == 8);

			THEN("the double precision floating-point value is written correctly")
			{
				const auto expected = std::array<std::byte, 8>{
					std::byte{0x40}, std::byte{0x1a}, std::byte{0x2c}, std::byte{0x08},
					std::byte{0x31}, std::byte{0x26}, std::byte{0xe9}, std::byte{0x79},
				};
				CHECK(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}
		}

		WHEN("seeking within the buffer")
		{
			for (std::size_t i = 0; i < data_buffer.size(); ++i) {
				data_buffer[i] = std::byte(i);
			}

			writer.skip(5);
			CHECK(writer.position() == 5);

			writer.seek(2);
			CHECK(writer.position() == 2);
			writer << uint8_t(42);

			writer.seek(0);
			CHECK(writer.position() == 0);
			writer << uint8_t(99);

			writer.seek(data_buffer.size());
			CHECK(writer.position() == data_buffer.size());
			writer << uint8_t(123);

			writer.seek(data_buffer.size() + 1);
			CHECK(writer.position() == data_buffer.size());
			writer << uint8_t(321);

			THEN("the buffer is updated")
			{
				CHECK(data_buffer[0] == std::byte{99});
				CHECK(data_buffer[2] == std::byte{42});

				for (std::size_t i = 0; i < data_buffer.size(); ++i) {
					if (i != 0 && i != 2) {
						CHECK(data_buffer[i] == static_cast<std::byte>(i));
					}
				}
			}
		}
	}
}

SCENARIO("ByteWriter: write data [little endian]")
{
	GIVEN("an ByteWriter")
	{
		std::array<std::byte, 21> data_buffer{};
		auto writer = ByteWriter{data_buffer, std::endian::little};

		WHEN("writing boolean values")
		{
			writer.skip(7);
			CHECK(writer.position() == 7);

			bool data = true;
			writer << data;
			CHECK(writer.position() == 8);

			writer.skip(12);
			writer << data;
			CHECK(writer.position() == 21);

			THEN("the boolean values are written correctly")
			{
				CHECK(data_buffer[7] == std::byte{0x01});
				CHECK(data_buffer[20] == std::byte{0x01});
			}
		}

		WHEN("writing unsigned integers")
		{
			const uint32_t data32 = 0x12345678;
			const uint16_t data16 = 0x7654;
			const uint8_t data8 = 0x98;

			writer << data32 << data8 << data16;
			CHECK(writer.position() == 7);

			THEN("the unsigned integers are written correctly")
			{
				const auto expected = std::array<std::byte, 7>{
					std::byte{0x78}, std::byte{0x56}, std::byte{0x34}, std::byte{0x12},
					std::byte{0x98}, std::byte{0x54}, std::byte{0x76},
				};
				CHECK(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}
		}

		WHEN("writing signed integers")
		{
			const int32_t data32 = 0x12345678;
			const int16_t data16 = 0x7654;
			const int8_t data8 = -104;

			writer << data32 << data8 << data16;
			CHECK(writer.position() == 7);

			THEN("the signed integers are written correctly")
			{
				const auto expected = std::array<std::byte, 7>{
					std::byte{0x78}, std::byte{0x56}, std::byte{0x34}, std::byte{0x12},
					std::byte{0x98}, std::byte{0x54}, std::byte{0x76},
				};
				CHECK(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}
		}

		WHEN("writing single precision floating-point values")
		{
			float data = 6.543F;
			writer << data;
			CHECK(writer.position() == 4);

			THEN("the single precision floating-point value is written correctly")
			{
				const auto expected = std::array<std::byte, 4>{
					std::byte{0x42},
					std::byte{0x60},
					std::byte{0xd1},
					std::byte{0x40},
				};
				CHECK(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}
		}

		WHEN("writing double precision floating-point values")
		{
			double data = 6.543;
			writer << data;
			CHECK(writer.position() == 8);

			THEN("the double precision floating-point value is written correctly")
			{
				const auto expected = std::array<std::byte, 8>{
					std::byte{0x79}, std::byte{0xe9}, std::byte{0x26}, std::byte{0x31},
					std::byte{0x08}, std::byte{0x2c}, std::byte{0x1a}, std::byte{0x40},
				};
				CHECK(data_buffer.size() >= expected.size());
				const auto data_view = std::span{data_buffer}.subspan(0, expected.size());
				REQUIRE_THAT(data_view, Catch::Matchers::RangeEquals(expected));
			}
		}

		WHEN("writing past the end of the buffer")
		{
			// Prefill the data buffer with sequential values
			for (std::size_t i = 0; i < data_buffer.size(); ++i) {
				data_buffer[i] = std::byte(i);
			}

			auto window = std::span{data_buffer.begin(), 5};
			auto window_writer = ByteWriter{window, std::endian::little};

			// Fill the window with new values
			for (std::size_t i = 0; i < window.size(); ++i) {
				window_writer << uint8_t(100 + i);
			}

			// Attempt to write past the end of the window
			CHECK(window_writer.position() == 5);
			window_writer << uint8_t(200);
			CHECK(window_writer.position() == 5);

			THEN("no data is written")
			{
				for (std::size_t i = 0; i < data_buffer.size(); ++i) {
					if (i < window.size()) {
						CHECK(data_buffer[i] == std::byte(100 + i));
					} else {
						CHECK(data_buffer[i] == std::byte(i));
					}
				}
			}
		}
	}
}
