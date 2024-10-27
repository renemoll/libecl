
#include <atomic>
#include <catch2/catch_all.hpp>

#include "catch2/catch_test_macros.hpp"
#include "containers/fifo.hpp"

using namespace ecl::common;

static_assert(std::is_default_constructible_v<Fifo<uint8_t, 6>>);
static_assert(std::is_nothrow_constructible_v<Fifo<uint8_t, 6>>);

static_assert(std::is_default_constructible_v<Fifo<uint8_t, 6>>);
static_assert(std::is_nothrow_default_constructible_v<Fifo<uint8_t, 6>>);

static_assert(std::is_copy_constructible_v<Fifo<uint8_t, 6>> == false);
static_assert(std::is_copy_assignable_v<Fifo<uint8_t, 6>> == false);

static_assert(std::is_move_constructible_v<Fifo<uint8_t, 6>> == false);
static_assert(std::is_nothrow_move_constructible_v<Fifo<uint8_t, 6>> == false);

static_assert(std::is_move_assignable_v<Fifo<uint8_t, 6>> == false);
static_assert(std::is_nothrow_move_assignable_v<Fifo<uint8_t, 6>> == false);

SCENARIO("FIFO: initialization")
{
	WHEN("constructing a FIFO")
	{
		Fifo<uint8_t, 6> dut{};

		THEN("capacity matches with the fixed size")
		{
			REQUIRE(dut.capacity() == 6U);
		}
		THEN("size is zero")
		{
			REQUIRE(dut.size() == 0);
		}
		THEN("fifo is empty")
		{
			REQUIRE(dut.empty());
		}
	}
}

SCENARIO("FIFO: adding data")
{
	GIVEN("an empty FIFO")
	{
		Fifo<int, 6> dut{};

		WHEN("pushing a single element")
		{
			REQUIRE(dut.push(1));

			THEN("the element is stored")
			{
				REQUIRE(dut.front() == 1);
			}
			THEN("size is one")
			{
				REQUIRE(dut.size() == 1);
			}
			THEN("fifo is not empty")
			{
				REQUIRE_FALSE(dut.empty());
			}
		}

		WHEN("pushing data untill full")
		{
			for (int i = 0; i < dut.capacity(); ++i) {
				REQUIRE(dut.push(i + 1));

				THEN("the size increases")
				{
					REQUIRE(dut.size() == (i + 1));
				}
				THEN("the front remains intact")
				{
					REQUIRE(dut.front() == 1);
				}
			}

			THEN("size matches capacity")
			{
				REQUIRE(dut.size() == dut.capacity());
			}
			THEN("fifo is not empty")
			{
				REQUIRE_FALSE(dut.empty());
			}
			THEN("pushing a new entry fails")
			{
				REQUIRE_FALSE(dut.push(9));
			}
		}

		WHEN("emplacing data untill full")
		{
			for (int i = 0; i < dut.capacity(); ++i) {
				REQUIRE(dut.emplace(i + 1));

				THEN("the size increases")
				{
					REQUIRE(dut.size() == (i + 1));
				}
				THEN("the front remains intact")
				{
					REQUIRE(dut.front() == 1);
				}
			}

			THEN("size matches capacity")
			{
				REQUIRE(dut.size() == dut.capacity());
			}
			THEN("fifo is not empty")
			{
				REQUIRE_FALSE(dut.empty());
			}
			THEN("emplacing a new entry fails")
			{
				REQUIRE_FALSE(dut.emplace(9));
			}
		}
	}
}

SCENARIO("FIFO: removing data")
{
	GIVEN("an empty FIFO")
	{
		Fifo<uint8_t, 6> dut{};

		WHEN("popping data")
		{
			REQUIRE_FALSE(dut.pop());
			THEN("fifo remains empty")
			{
				REQUIRE(dut.size() == 0);
				REQUIRE(dut.empty());
			}
		}
	}

	GIVEN("a FIFO contating data")
	{
		Fifo<uint8_t, 6> dut{};
		for (int i = 0; i < dut.capacity(); ++i) {
			dut.push(i + 1);
		}

		WHEN("front is called")
		{
			THEN("the oldest entry is retruned")
			{
				REQUIRE(dut.front() == 1);
			}
			THEN("size remains unchanged")
			{
				REQUIRE(dut.size() == dut.capacity());
			}
		}

		WHEN("pop is called")
		{
			REQUIRE(dut.pop());

			THEN("the oldest entry is removed")
			{
				REQUIRE(dut.front() == 2);
			}
			THEN("size decreases")
			{
				REQUIRE(dut.size() < dut.capacity());
			}
		}

		WHEN("pop is called until empty")
		{
			REQUIRE(dut.size() == 6);

			for (int i = 0; i < dut.capacity(); ++i) {
				THEN("front retruns the oldest entry")
				{
					REQUIRE(dut.front() == (i + 1));
				}

				REQUIRE(dut.pop());

				THEN("size decreases")
				{
					REQUIRE(dut.size() < dut.capacity());
				}
			}

			THEN("FIFO is empty")
			{
				REQUIRE(dut.empty());
			}
		}
	}
}

std::atomic<int> allocations = 0;
std::atomic<int> deallocations = 0;

SCENARIO("FIFO: store objects")
{
	allocations = 0;
	deallocations = 0;

	class Element
	{
	public:
		Element()
			: m_i{0}
		{
			allocations++;
		}
		Element(int i)
			: m_i{i}
		{
			allocations++;
		}
		~Element()
		{
			deallocations++;
		}

		int m_i;
	};

	GIVEN("an empty FIFO")
	{
		Fifo<Element, 6> dut{};

		WHEN("pushing data to the FIFO")
		{
			REQUIRE(dut.push(Element{2}));
			REQUIRE(dut.push(Element{}));
			REQUIRE(dut.push(Element{4}));

			THEN("size matches")
			{
				REQUIRE(dut.size() == 3);
			}
			THEN("number of allocations match calls to push")
			{
				REQUIRE(allocations == 3);
			}

			WHEN("calling clear")
			{
				dut.clear();

				THEN("fifo is empty")
				{
					REQUIRE(dut.empty());
				}
				THEN("number of deallocations match the number of allocations")
				{
					REQUIRE(deallocations == 3);
				}
			}
		}

		allocations = 0;
		deallocations = 0;

		WHEN("creating objects in place")
		{
			REQUIRE(dut.emplace(6));

			THEN("value can be retrieved")
			{
				REQUIRE(dut.front().m_i == 6);
			}
			THEN("number of allocations match calls to emplace")
			{
				REQUIRE(allocations == 1);
			}

			WHEN("calling pop")
			{
				REQUIRE(dut.pop());

				THEN("number of deallocations match the number of allocations")
				{
					REQUIRE(deallocations == 1);
				}
			}
		}
	}
}

SCENARIO("FIFO: store objects without default constructors")
{
	allocations = 0;
	deallocations = 0;

	class Element
	{
	public:
		Element() = delete;
		Element(int i)
			: m_i{i}
		{
			allocations++;
		}
		~Element()
		{
			deallocations++;
		}

		int m_i;
	};

	GIVEN("an empty FIFO")
	{
		Fifo<Element, 6> dut{};

		WHEN("pushing data to the FIFO")
		{
			REQUIRE(dut.push(Element{2}));
			REQUIRE(dut.push(Element{4}));

			THEN("size matches")
			{
				REQUIRE(dut.size() == 2);
			}
			THEN("number of allocations match calls to push")
			{
				REQUIRE(allocations == 2);
			}

			WHEN("calling clear")
			{
				dut.clear();

				THEN("fifo is empty")
				{
					REQUIRE(dut.empty());
				}
				THEN("number of deallocations match the number of allocations")
				{
					REQUIRE(deallocations == 2);
				}
			}
		}

		allocations = 0;
		deallocations = 0;

		WHEN("creating objects in place")
		{
			REQUIRE(dut.emplace(6));

			THEN("value can be retrieved")
			{
				REQUIRE(dut.front().m_i == 6);
			}
			THEN("number of allocations match calls to emplace")
			{
				REQUIRE(allocations == 1);
			}

			WHEN("calling pop")
			{
				REQUIRE(dut.pop());

				THEN("number of deallocations match the number of allocations")
				{
					REQUIRE(deallocations == 1);
				}
			}
		}
	}
}
