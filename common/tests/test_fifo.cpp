
#include <atomic>
#include <catch2/catch_all.hpp>

#include "catch2/catch_test_macros.hpp"
#include "containers/fifo.hpp"

using namespace ecl::common;

static_assert(std::is_default_constructible_v<Fifo<uint8_t, 6>>);
// static_assert(std::is_trivially_constructible_v<Fifo<uint8_t, 6>>);
static_assert(std::is_nothrow_constructible_v<Fifo<uint8_t, 6>>);

static_assert(std::is_default_constructible_v<Fifo<uint8_t, 6>>);
// static_assert(std::is_trivially_default_constructible_v<Fifo<uint8_t, 6>>);
static_assert(std::is_nothrow_default_constructible_v<Fifo<uint8_t, 6>>);

static_assert(std::is_copy_constructible_v<Fifo<uint8_t, 6>> == false);
static_assert(std::is_trivially_copy_constructible_v<Fifo<uint8_t, 6>> ==
			  false);
static_assert(std::is_nothrow_copy_constructible_v<Fifo<uint8_t, 6>> == false);

// static_assert(std::is_move_constructible_v<Fifo<uint8_t, 6>>);
// static_assert(std::is_trivially_move_constructible_v<Fifo<uint8_t, 6>>);
// static_assert(std::is_nothrow_move_constructible_v<Fifo<uint8_t, 6>>);

static_assert(std::is_copy_assignable_v<Fifo<uint8_t, 6>> == false);
static_assert(std::is_trivially_copy_assignable_v<Fifo<uint8_t, 6>> == false);
static_assert(std::is_nothrow_copy_assignable_v<Fifo<uint8_t, 6>> == false);

// static_assert(std::is_move_assignable_v<Fifo<uint8_t, 6>>);
// static_assert(std::is_trivially_move_assignable_v<Fifo<uint8_t, 6>>);
// static_assert(std::is_nothrow_move_assignable_v<Fifo<uint8_t, 6>>);

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
