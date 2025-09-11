/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/containers/queue.hpp"

#include <cstdint>
#include <type_traits>

#include "catch2/catch_test_macros.hpp"

using namespace libecl::containers;

SCENARIO("Queue: type traits")
{
	using Q = Queue<uint8_t, 6>;

	static_assert(std::is_constructible_v<Q>);
	static_assert(std::is_nothrow_constructible_v<Q>);

	static_assert(std::is_default_constructible_v<Q>);
	static_assert(std::is_nothrow_default_constructible_v<Q>);

	static_assert(!std::is_copy_constructible_v<Q>);
	static_assert(!std::is_copy_assignable_v<Q>);

	static_assert(!std::is_move_constructible_v<Q>);
	static_assert(!std::is_nothrow_move_constructible_v<Q>);

	static_assert(!std::is_move_assignable_v<Q>);
	static_assert(!std::is_nothrow_move_assignable_v<Q>);
}

SCENARIO("Queue: initialization")
{
	WHEN("constructing a queue")
	{
		Queue<uint8_t, 6> dut{};

		THEN("capacity matches the fixed size")
		{
			REQUIRE(dut.capacity() == 6U);
		}
		THEN("queue is empty")
		{
			REQUIRE(dut.empty());
		}

		THEN("pop fails")
		{
			REQUIRE_FALSE(dut.pop());
		}
	}
}

SCENARIO("Queue: push/pop")
{
	GIVEN("an empty queue")
	{
		Queue<std::size_t, 7> dut{};

		WHEN("pushing a single element")
		{
			REQUIRE(dut.push(1));

			THEN("the element is stored")
			{
				REQUIRE(dut.front() == 1);
			}
			THEN("queue is not empty")
			{
				REQUIRE_FALSE(dut.empty());
			}
		}

		WHEN("pushing data until full")
		{
			for (std::size_t i = 0; i < dut.capacity(); ++i) {
				REQUIRE(dut.push(i + 1));

				THEN("the front remains intact")
				{
					REQUIRE(dut.front() == 1);
				}
			}

			THEN("no new data can be queued")
			{
				REQUIRE_FALSE(dut.push(0));
			}

			THEN("queue is not empty")
			{
				REQUIRE_FALSE(dut.empty());
			}

			for (std::size_t i = 0; i < dut.capacity(); ++i) {
				REQUIRE(dut.front() == (i + 1));
				dut.pop();
			}
		}
	}

	GIVEN("a full queue")
	{
		Queue<std::size_t, 6> dut{};
		for (std::size_t i = 0; i < dut.capacity(); ++i) {
			REQUIRE(dut.push(i + 1));
		}
		REQUIRE(dut.front() == 1);

		WHEN("pushing additional data")
		{
			THEN("push fails")
			{
				REQUIRE_FALSE(dut.push(99));
			}
			THEN("the oldest entry is retained")
			{
				REQUIRE(dut.front() == 1);
			}
		}

		WHEN("only after data is popped")
		{
			REQUIRE(dut.pop());

			THEN("the oldest entry is removed")
			{
				REQUIRE(dut.front() == 2);
			}

			THEN("new data can be pushed to the queue")
			{
				REQUIRE(dut.push(dut.capacity()));
				REQUIRE_FALSE(dut.push(dut.capacity() + 1));
			}
		}

		WHEN("pop is called until empty")
		{
			REQUIRE_FALSE(dut.push(0));

			for (std::size_t i = 0; i < dut.capacity(); ++i) {
				THEN("front retruns the oldest entry")
				{
					REQUIRE(dut.front() == (i + 1));
				}

				REQUIRE(dut.pop());
			}

			THEN("queue is empty")
			{
				REQUIRE(dut.empty());
				REQUIRE_FALSE(dut.pop());
			}
		}
	}
}

SCENARIO("Queue: emplace")
{
	GIVEN("an empty queue")
	{
		Queue<std::size_t, 3> dut{};

		WHEN("pushing a single element")
		{
			REQUIRE(dut.emplace(1));

			THEN("the element is stored")
			{
				REQUIRE(dut.front() == 1);
			}
			THEN("queue is not empty")
			{
				REQUIRE_FALSE(dut.empty());
			}
		}

		WHEN("emplacing data until full")
		{
			for (std::size_t i = 0; i < dut.capacity(); ++i) {
				REQUIRE(dut.emplace(i + 1));

				THEN("the front remains intact")
				{
					REQUIRE(dut.front() == 1);
				}
			}

			THEN("no new data can be queued")
			{
				REQUIRE_FALSE(dut.push(0));
			}

			THEN("queue is not empty")
			{
				REQUIRE_FALSE(dut.empty());
			}

			for (std::size_t i = 0; i < dut.capacity(); ++i) {
				REQUIRE(dut.front() == (i + 1));
				dut.pop();
			}
		}
	}

	GIVEN("a full queue")
	{
		Queue<std::size_t, 6> dut{};
		for (std::size_t i = 0; i < dut.capacity(); ++i) {
			REQUIRE(dut.push(i + 1));
		}
		REQUIRE(dut.front() == 1);

		WHEN("emplacing additional data")
		{
			THEN("emplace fails")
			{
				REQUIRE(dut.emplace(99U) == nullptr);
			}
			THEN("the oldest entry is overwritten")
			{
				REQUIRE(dut.front() == 1);
			}
		}

		WHEN("only after data is popped")
		{
			REQUIRE(dut.pop());

			THEN("the oldest entry is removed")
			{
				REQUIRE(dut.front() == 2);
			}

			THEN("new data can be pushed to the queue")
			{
				REQUIRE(dut.emplace(dut.capacity()));
				REQUIRE(dut.emplace(dut.capacity() + 1) == nullptr);
			}
		}
	}
}

int allocations = 0;
int deallocations = 0;
int copied = 0;
int moved = 0;

SCENARIO("Queue: store objects")
{
	allocations = 0;
	deallocations = 0;
	copied = 0;
	moved = 0;

	class Element
	{
	public:
		Element()
			: m_i{0}
		{
			allocations++;
		}

		explicit Element(int i)
			: m_i{i}
		{
			allocations++;
		}

		~Element()
		{
			deallocations++;
		}

		Element(const Element& obj)
			: m_i{obj.m_i}
		{
			copied++;
		}

		Element& operator=(const Element& other) = default;

		Element(Element&& obj) noexcept
			: m_i{obj.m_i}
		{
			moved++;
		}

		Element& operator=(Element&& other) noexcept = default;

		int m_i;
	};
	static_assert(std::is_default_constructible_v<Element>);
	static_assert(std::is_copy_constructible_v<Element>);
	static_assert(std::is_move_constructible_v<Element>);

	GIVEN("an empty queue")
	{
		Queue<Element, 6> dut{};

		WHEN("pushing data to the queue")
		{
			REQUIRE(dut.push(Element{2}));
			REQUIRE(dut.push(Element{}));
			REQUIRE(dut.push(Element{4}));

			THEN("number of allocations match calls to push")
			{
				REQUIRE(allocations == 3);
				REQUIRE(deallocations == 3);
				REQUIRE(copied == 0);
				REQUIRE(moved == 3);
			}

			WHEN("calling clear")
			{
				dut.clear();

				THEN("queue is empty")
				{
					REQUIRE(dut.empty());
				}
				THEN("number of deallocations match the number of allocations")
				{
					REQUIRE(allocations == 3);
					REQUIRE(deallocations == (allocations + moved));
					REQUIRE(copied == 0);
					REQUIRE(moved == 3);
				}
			}
		}

		allocations = 0;
		deallocations = 0;
		copied = 0;
		moved = 0;

		WHEN("creating objects in place")
		{
			const auto* result = dut.emplace(6);
			REQUIRE(result != nullptr);
			REQUIRE(result->m_i == 6);

			THEN("value can be retrieved")
			{
				REQUIRE(dut.front().m_i == 6);
			}
			THEN("number of allocations match calls to emplace")
			{
				REQUIRE(allocations == 1);
				REQUIRE(deallocations == 0);
				REQUIRE(copied == 0);
				REQUIRE(moved == 0);
			}

			WHEN("calling pop")
			{
				REQUIRE(dut.pop());

				THEN("number of deallocations match the number of allocations")
				{
					REQUIRE(allocations == 1);
					REQUIRE(deallocations == 1);
					REQUIRE(copied == 0);
					REQUIRE(moved == 0);
				}
			}
		}
	}
}

SCENARIO("Queue: store objects without default constructors")
{
	allocations = 0;
	deallocations = 0;
	copied = 0;
	moved = 0;

	class Element
	{
	public:
		explicit Element(int i)
			: m_i{i}
		{
			allocations++;
		}

		~Element()
		{
			deallocations++;
		}

		Element(const Element& obj)
			: m_i{obj.m_i}
		{
			copied++;
		}

		Element& operator=(const Element& other) = default;

		Element(Element&& obj) noexcept
			: m_i{obj.m_i}
		{
			moved++;
		}

		Element& operator=(Element&& other) noexcept = default;

		int m_i;
	};
	static_assert(!std::is_default_constructible_v<Element>);
	static_assert(std::is_copy_constructible_v<Element>);
	static_assert(std::is_move_constructible_v<Element>);

	GIVEN("an empty queue")
	{
		Queue<Element, 6> dut{};

		WHEN("pushing data to the queue")
		{
			REQUIRE(dut.push(Element{2}));
			REQUIRE(dut.push(Element{4}));

			THEN("number of allocations match calls to push")
			{
				REQUIRE(allocations == 2);
				REQUIRE(deallocations == 2);
				REQUIRE(copied == 0);
				REQUIRE(moved == 2);
			}

			WHEN("calling clear")
			{
				dut.clear();

				THEN("queue is empty")
				{
					REQUIRE(dut.empty());
				}
				THEN("number of deallocations match the number of allocations")
				{
					REQUIRE(allocations == 2);
					REQUIRE(deallocations == (allocations + moved));
					REQUIRE(copied == 0);
					REQUIRE(moved == 2);
				}
			}
		}

		allocations = 0;
		deallocations = 0;
		copied = 0;
		moved = 0;

		WHEN("creating objects in place")
		{
			const auto* result = dut.emplace(6);
			REQUIRE(result != nullptr);
			REQUIRE(result->m_i == 6);

			THEN("value can be retrieved")
			{
				REQUIRE(dut.front().m_i == 6);
			}
			THEN("number of allocations match calls to emplace")
			{
				REQUIRE(allocations == 1);
				REQUIRE(deallocations == 0);
				REQUIRE(copied == 0);
				REQUIRE(moved == 0);
			}

			WHEN("calling pop")
			{
				REQUIRE(dut.pop());

				THEN("number of deallocations match the number of allocations")
				{
					REQUIRE(allocations == 1);
					REQUIRE(deallocations == 1);
					REQUIRE(copied == 0);
					REQUIRE(moved == 0);
				}
			}
		}
	}
}
