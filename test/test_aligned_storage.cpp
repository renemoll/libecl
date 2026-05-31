/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/containers/aligned_storage.hpp"

#include <type_traits>

#include "catch2/catch_test_macros.hpp"

using namespace libecl::containers;

SCENARIO("AlignedStorage: type traits")
{
	static_assert(std::is_trivially_destructible_v<AlignedStorage<int>>);
}

SCENARIO("AlignedStorage: empty")
{
	GIVEN("an empty AlignedStorage")
	{
		AlignedStorage<int> dut{};

		THEN("data is empty")
		{
			REQUIRE(dut.data() == nullptr);
		}

		WHEN("destroying the empty buffer")
		{
			dut.destroy();

			THEN("data is still empty")
			{
				REQUIRE(dut.data() == nullptr);
			}
		}
	}
}

SCENARIO("AlignedStorage: trivial type")
{
	GIVEN("an empty AlignedStorage")
	{
		AlignedStorage<int> dut{};

		WHEN("storing a value")
		{
			dut.store(42);

			THEN("data is stored in the buffer")
			{
				REQUIRE(dut.data() != nullptr);
				REQUIRE(*dut.data() == 42);
			}

			WHEN("destroying the value")
			{
				dut.destroy();

				THEN("data is empty")
				{
					REQUIRE(dut.data() == nullptr);
				}
			}

			WHEN("storing another value")
			{
				dut.store(99);

				THEN("data is updated in the buffer")
				{
					REQUIRE(dut.data() != nullptr);
					REQUIRE(*dut.data() == 99);
				}
			}
		}
	}
}

SCENARIO("AlignedStorage: non-copyable, non-movable type")
{
	class NonCopyableNonMovable
	{
	public:
		NonCopyableNonMovable() = default;
		~NonCopyableNonMovable() = default;

		NonCopyableNonMovable(const NonCopyableNonMovable&) = delete;
		NonCopyableNonMovable& operator=(const NonCopyableNonMovable&) = delete;

		NonCopyableNonMovable(NonCopyableNonMovable&&) = delete;
		NonCopyableNonMovable& operator=(NonCopyableNonMovable&&) = delete;
	};

	GIVEN("an empty AlignedStorage for a non-copyable, non-movable type")
	{
		AlignedStorage<NonCopyableNonMovable> dut{};

		THEN("a value is emplaced in the buffer")
		{
			dut.emplace();

			REQUIRE(dut.data() != nullptr);
		}

		WHEN("destroying the value")
		{
			dut.destroy();

			THEN("data is empty")
			{
				REQUIRE(dut.data() == nullptr);
			}
		}
	}
}
