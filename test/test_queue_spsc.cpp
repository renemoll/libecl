/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/containers/queue_spsc.hpp"

#include <type_traits>
#include <utility>

#include "catch2/catch_test_macros.hpp"

using namespace libecl::containers;

SCENARIO("QueueSpsc: type traits")
{
	constexpr std::size_t capacity = 2;
	using T = QueueSpsc<uint8_t, capacity>;

	static_assert(std::is_constructible_v<T>);
	static_assert(std::is_nothrow_constructible_v<T>);

	static_assert(std::is_default_constructible_v<T>);
	static_assert(std::is_nothrow_default_constructible_v<T>);

	static_assert(!std::is_copy_constructible_v<T>);
	static_assert(!std::is_copy_assignable_v<T>);

	static_assert(!std::is_move_constructible_v<T>);
	static_assert(!std::is_nothrow_move_constructible_v<T>);

	static_assert(!std::is_move_assignable_v<T>);
	static_assert(!std::is_nothrow_move_assignable_v<T>);

	struct CopyConstructionOnly
	{
		CopyConstructionOnly() = default;
		CopyConstructionOnly(const CopyConstructionOnly&) noexcept = default;
		CopyConstructionOnly& operator=(const CopyConstructionOnly&) = delete;
		CopyConstructionOnly(CopyConstructionOnly&&) = delete;
		CopyConstructionOnly& operator=(CopyConstructionOnly&&) = delete;
	};
	static_assert(std::is_nothrow_copy_constructible_v<CopyConstructionOnly>);
	static_assert(!std::is_nothrow_copy_assignable_v<CopyConstructionOnly>);
	static_assert(!std::is_nothrow_move_constructible_v<CopyConstructionOnly>);
	static_assert(!std::is_nothrow_move_assignable_v<CopyConstructionOnly>);

	using CopyQueue = QueueSpsc<CopyConstructionOnly, capacity>;
	static_assert(requires(CopyQueue& q, const CopyConstructionOnly& value) { q.push(value); });

	struct MoveConstructionOnly
	{
		MoveConstructionOnly() = default;
		MoveConstructionOnly(const MoveConstructionOnly&) = delete;
		MoveConstructionOnly& operator=(const MoveConstructionOnly&) = delete;
		MoveConstructionOnly(MoveConstructionOnly&&) noexcept = default;
		MoveConstructionOnly& operator=(MoveConstructionOnly&&) = delete;
	};
	static_assert(std::is_nothrow_move_constructible_v<MoveConstructionOnly>);
	static_assert(!std::is_nothrow_move_assignable_v<MoveConstructionOnly>);

	using MoveQueue = QueueSpsc<MoveConstructionOnly, capacity>;
	static_assert(requires(MoveQueue& q, MoveConstructionOnly&& value) { q.push(std::move(value)); });
	static_assert(requires(MoveQueue& q) { q.emplace(); });

	struct ThrowingMoveAssign
	{
		ThrowingMoveAssign() = default;
		ThrowingMoveAssign(const ThrowingMoveAssign&) = delete;
		ThrowingMoveAssign& operator=(const ThrowingMoveAssign&) = delete;
		ThrowingMoveAssign(ThrowingMoveAssign&&) noexcept = default;
		ThrowingMoveAssign& operator=(ThrowingMoveAssign&&) noexcept(false)
		{
			return *this;
		}
	};
	static_assert(std::is_nothrow_move_constructible_v<ThrowingMoveAssign>);
	static_assert(!std::is_nothrow_move_assignable_v<ThrowingMoveAssign>);

	using ThrowingAssignQueue = QueueSpsc<ThrowingMoveAssign, capacity>;
	static_assert(requires(ThrowingAssignQueue& q, ThrowingMoveAssign&& value) { q.push(std::move(value)); });
}

SCENARIO("QueueSpsc: initialization")
{
	WHEN("constructing a queue")
	{
		constexpr std::size_t capacity = 6;
		QueueSpsc<uint8_t, capacity> dut{};

		THEN("capacity matches the fixed size")
		{
			REQUIRE(dut.capacity() == capacity);
		}
		THEN("queue is empty")
		{
			REQUIRE(dut.empty());
			REQUIRE_FALSE(dut.full());
		}

		THEN("pop fails")
		{
			uint8_t value = 0;
			REQUIRE_FALSE(dut.pop(value));
		}
	}
}

SCENARIO("QueueSpsc: push/pop")
{
	GIVEN("an empty queue")
	{
		constexpr std::size_t capacity = 7;
		QueueSpsc<std::size_t, capacity> dut{};

		WHEN("pushing a single element")
		{
			REQUIRE(dut.empty());
			REQUIRE(dut.push(1));

			// THEN("the element is stored")
			// {
			// 	REQUIRE(dut.front() == 1);
			// }

			THEN("queue is not empty, nor full")
			{
				REQUIRE_FALSE(dut.empty());
				REQUIRE_FALSE(dut.full());
			}
		}

		WHEN("pushing data until full")
		{
			for (std::size_t i = 0; i < dut.capacity(); ++i) {
				REQUIRE(dut.push(i + 1));
				REQUIRE_FALSE(dut.empty());

				// THEN("the front remains intact")
				// {
				// 	REQUIRE(dut.front() == 1);
				// }
			}

			THEN("no new data can be queued")
			{
				REQUIRE_FALSE(dut.push(0));
			}

			THEN("queue full and not not empty")
			{
				REQUIRE(dut.full());
				REQUIRE_FALSE(dut.empty());
			}

			THEN("queue contains the expected sequence")
			{
				std::size_t value = 0;
				for (std::size_t i = 0; i < dut.capacity(); ++i) {
					// REQUIRE(dut.front() == (i + 1));
					dut.pop(value);
					REQUIRE(value == (i + 1));
				}
			}
		}
	}

	GIVEN("a full queue")
	{
		constexpr std::size_t capacity = 6;
		QueueSpsc<std::size_t, capacity> dut{};
		for (std::size_t i = 0; i < dut.capacity(); ++i) {
			REQUIRE(dut.push(i + 1));
		}
		// REQUIRE(dut.front() == 1);
		REQUIRE(dut.full());
		REQUIRE_FALSE(dut.empty());

		WHEN("pushing additional data")
		{
			THEN("push fails")
			{
				REQUIRE_FALSE(dut.push(99));
			}

			// THEN("the oldest entry is retained")
			// {
			// 	REQUIRE(dut.front() == 1);
			// }
		}

		WHEN("only after data is popped")
		{
			std::size_t value = 0;
			REQUIRE(dut.pop(value));
			REQUIRE(value == 1);

			REQUIRE_FALSE(dut.full());
			REQUIRE_FALSE(dut.empty());

			// THEN("the oldest entry is removed")
			// {
			// 	REQUIRE(dut.front() == 2);
			// }

			THEN("new data can be pushed to the queue")
			{
				REQUIRE(dut.push(dut.capacity()));
				REQUIRE(dut.full());
				REQUIRE_FALSE(dut.push(dut.capacity() + 1));
			}
		}

		WHEN("pop is called until empty")
		{
			REQUIRE_FALSE(dut.push(0));
			REQUIRE(dut.full());

			std::size_t value = 0;
			for (std::size_t i = 0; i < dut.capacity(); ++i) {
				REQUIRE_FALSE(dut.empty());
				// THEN("front retruns the oldest entry")
				// {
				// 	REQUIRE(dut.front() == (i + 1));
				// }

				REQUIRE(dut.pop(value));
				REQUIRE(value == (i + 1));
				REQUIRE_FALSE(dut.full());
			}

			THEN("queue is empty")
			{
				REQUIRE(dut.empty());
				REQUIRE_FALSE(dut.pop(value));
			}
		}
	}
}

SCENARIO("QueueSpsc: emplace")
{
	GIVEN("an empty queue")
	{
		constexpr std::size_t capacity = 3;
		QueueSpsc<std::size_t, capacity> dut{};

		WHEN("emplacing a single element")
		{
			REQUIRE(dut.emplace(1U));

			// THEN("the element is stored")
			// {
			// 	REQUIRE(dut.front() == 1);
			// }

			THEN("queue is not empty, nor full")
			{
				REQUIRE_FALSE(dut.empty());
				REQUIRE_FALSE(dut.full());
			}
		}

		WHEN("emplacing data until full")
		{
			for (std::size_t i = 0; i < dut.capacity(); ++i) {
				REQUIRE(dut.emplace(i + 1U));
				REQUIRE_FALSE(dut.empty());

				// THEN("the front remains intact")
				// {
				// 	REQUIRE(dut.front() == 1);
				// }
			}

			THEN("no new data can be queued")
			{
				REQUIRE_FALSE(dut.push(0));
			}

			THEN("queue full and not not empty")
			{
				REQUIRE(dut.full());
				REQUIRE_FALSE(dut.empty());
			}

			THEN("queue contains the expected sequence")
			{
				std::size_t value = 0;
				for (std::size_t i = 0; i < dut.capacity(); ++i) {
					// REQUIRE(dut.front() == (i + 1));
					dut.pop(value);
					REQUIRE(value == (i + 1));
				}
			}
		}
	}

	GIVEN("a full queue")
	{
		constexpr std::size_t capacity = 6;
		QueueSpsc<std::size_t, capacity> dut{};
		for (std::size_t i = 0; i < dut.capacity(); ++i) {
			REQUIRE(dut.push(i + 1));
		}
		// REQUIRE(dut.front() == 1);
		REQUIRE(dut.full());
		REQUIRE_FALSE(dut.empty());

		WHEN("emplacing additional data")
		{
			THEN("emplace fails")
			{
				REQUIRE(dut.emplace(99U) == nullptr);
			}

			// THEN("the oldest entry is overwritten")
			// {
			// 	REQUIRE(dut.front() == 1);
			// }
		}

		WHEN("only after data is popped")
		{
			std::size_t value = 0;
			REQUIRE(dut.pop(value));
			REQUIRE(value == 1);

			REQUIRE_FALSE(dut.full());
			REQUIRE_FALSE(dut.empty());

			// THEN("the oldest entry is removed")
			// {
			// 	REQUIRE(dut.front() == 2);
			// }

			THEN("new data can be pushed to the queue")
			{
				REQUIRE(dut.emplace(dut.capacity()));
				REQUIRE(dut.full());
				REQUIRE(dut.emplace(dut.capacity() + 1) == nullptr);
			}
		}
	}
}

int allocations = 0;
int deallocations = 0;
int copied = 0;
int moved = 0;

SCENARIO("QueueSpsc: store objects")
{
	allocations = 0;
	deallocations = 0;
	copied = 0;
	moved = 0;

	class Element
	{
	public:
		Element() noexcept
			: m_value{0}
		{
			allocations++;
		}

		explicit Element(int value) noexcept
			: m_value{value}
		{
			allocations++;
		}

		~Element() noexcept
		{
			deallocations++;
		}

		Element(const Element& obj) noexcept
			: m_value{obj.m_value}
		{
			copied++;
		}

		Element& operator=(const Element& other) noexcept = default;

		Element(Element&& obj) noexcept
			: m_value{obj.m_value}
		{
			moved++;
		}

		Element& operator=(Element&& other) noexcept = default;

		// NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
		int m_value;
	};
	static_assert(std::is_default_constructible_v<Element>);
	static_assert(std::is_copy_constructible_v<Element>);
	static_assert(std::is_move_constructible_v<Element>);

	GIVEN("an empty queue")
	{
		constexpr std::size_t capacity = 6;
		QueueSpsc<Element, capacity> dut{};

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
				REQUIRE(dut.empty());

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
			REQUIRE(result->m_value == 6);

			// THEN("value can be retrieved")
			// {
			// 	REQUIRE(dut.front().m_value == 6);
			// }
			THEN("number of allocations match calls to emplace")
			{
				REQUIRE(allocations == 1);
				REQUIRE(deallocations == 0);
				REQUIRE(copied == 0);
				REQUIRE(moved == 0);
			}

			WHEN("calling pop")
			{
				{
					auto value = Element{0};
					REQUIRE(dut.pop(value));
					REQUIRE(value.m_value == 6);
				}

				THEN("number of deallocations match the number of allocations")
				{
					REQUIRE(allocations == 2);
					REQUIRE(deallocations == 2);
					REQUIRE(copied == 0);
					REQUIRE(moved == 0);
				}
			}
		}
	}
}

SCENARIO("QueueSpsc: store objects without default constructors")
{
	allocations = 0;
	deallocations = 0;
	copied = 0;
	moved = 0;

	class Element
	{
	public:
		explicit Element(int value) noexcept
			: m_value{value}
		{
			allocations++;
		}

		~Element() noexcept
		{
			deallocations++;
		}

		Element(const Element& obj) noexcept
			: m_value{obj.m_value}
		{
			copied++;
		}

		Element& operator=(const Element& other) noexcept = default;

		Element(Element&& obj) noexcept
			: m_value{obj.m_value}
		{
			moved++;
		}

		Element& operator=(Element&& other) noexcept = default;

		// NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
		int m_value;
	};
	static_assert(!std::is_default_constructible_v<Element>);
	static_assert(std::is_copy_constructible_v<Element>);
	static_assert(std::is_move_constructible_v<Element>);

	GIVEN("an empty queue")
	{
		constexpr std::size_t capacity = 6;
		QueueSpsc<Element, capacity> dut{};

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
				REQUIRE(dut.empty());

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
			REQUIRE(result->m_value == 6);

			// THEN("value can be retrieved")
			// {
			// 	REQUIRE(dut.front().m_value == 6);
			// }
			THEN("number of allocations match calls to emplace")
			{
				REQUIRE(allocations == 1);
				REQUIRE(deallocations == 0);
				REQUIRE(copied == 0);
				REQUIRE(moved == 0);
			}

			WHEN("calling pop")
			{
				{
					auto value = Element{0};
					REQUIRE(dut.pop(value));
					REQUIRE(value.m_value == 6);
				}

				THEN("number of deallocations match the number of allocations")
				{
					REQUIRE(allocations == 2);
					REQUIRE(deallocations == 2);
					REQUIRE(copied == 0);
					REQUIRE(moved == 0);
				}
			}
		}
	}
}

SCENARIO("QueueSpsc: clear calls the destructor for non-trivially destructible types")
{
	allocations = 0;
	deallocations = 0;

	class Element
	{
	public:
		explicit Element(int value) noexcept
			: m_value{value}
		{
			allocations++;
		}

		~Element() noexcept
		{
			deallocations++;
		}

		Element(const Element&) = delete;
		Element& operator=(const Element&) = delete;
		Element(Element&&) noexcept = default;
		Element& operator=(Element&&) noexcept = default;

		int m_value;
	};

	{
		QueueSpsc<Element, 3> dut{};
		REQUIRE(dut.emplace(1) != nullptr);
		REQUIRE(dut.emplace(2) != nullptr);
		REQUIRE(dut.emplace(3) != nullptr);

		auto output = Element{0};
		REQUIRE(dut.pop(output));
		REQUIRE(output.m_value == 1);
		REQUIRE(dut.emplace(4) != nullptr);

		dut.clear();
		REQUIRE(dut.empty());
		REQUIRE(deallocations == 4);
	}

	REQUIRE(allocations == deallocations);
}

SCENARIO("QueueSpsc: destructor calls the destructor for non-trivially destructible types")
{
	allocations = 0;
	deallocations = 0;

	class Element
	{
	public:
		explicit Element(int value) noexcept
			: m_value{value}
		{
			allocations++;
		}

		~Element() noexcept
		{
			deallocations++;
		}

		Element(const Element&) = delete;
		Element& operator=(const Element&) = delete;
		Element(Element&&) noexcept = default;
		Element& operator=(Element&&) noexcept = default;

		int m_value;
	};

	{
		QueueSpsc<Element, 4> dut{};
		REQUIRE(dut.emplace(11) != nullptr);
		REQUIRE(dut.emplace(12) != nullptr);
		REQUIRE(dut.emplace(13) != nullptr);
		REQUIRE(allocations == 3);
		REQUIRE(deallocations == 0);
	}

	REQUIRE(allocations == deallocations);
}
