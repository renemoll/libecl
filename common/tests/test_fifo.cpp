
#include <catch2/catch_all.hpp>

#include "catch2/catch_test_macros.hpp"
#include "containers/fifo.hpp"

using namespace ecl_common;

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

SCENARIO("FIFO: pushing data")
{
	GIVEN("an empty FIFO")
	{
		Fifo<int, 6> dut{};

		WHEN("pushing a single element")
		{
			dut.push(1);

			THEN("size is one")
			{
				REQUIRE(dut.size() == 1);
			}
			THEN("fifo is not empty")
			{
				REQUIRE_FALSE(dut.empty());
			}
		}

		WHEN("filling the FIFO")
		{
			for (int i = 0; i < 6; ++i) {
				dut.push(i + 1);
				REQUIRE(dut.size() == (i + 1));
				REQUIRE_FALSE(dut.empty());
			}
		}
	}
}

SCENARIO("FIFO: popping data")
{
	GIVEN("an empty FIFO")
	{
		Fifo<uint8_t, 6> dut{};
	}
}

SCENARIO("FIFO: iterating") {}

// back inserter
// iterator
