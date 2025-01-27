#include "libecl/containers/fifo.hpp"

#include <cstdint>
#include <type_traits>

#include "catch2/catch_test_macros.hpp"

using namespace ecl::containers;

static_assert(std::is_default_constructible_v<Fifo<uint8_t, 6>>);
static_assert(std::is_nothrow_constructible_v<Fifo<uint8_t, 6>>);

static_assert(std::is_default_constructible_v<Fifo<uint8_t, 6>>);
static_assert(std::is_nothrow_default_constructible_v<Fifo<uint8_t, 6>>);

static_assert(!std::is_copy_constructible_v<Fifo<uint8_t, 6>>);
static_assert(!std::is_copy_assignable_v<Fifo<uint8_t, 6>>);

static_assert(!std::is_move_constructible_v<Fifo<uint8_t, 6>>);
static_assert(!std::is_nothrow_move_constructible_v<Fifo<uint8_t, 6>>);

static_assert(!std::is_move_assignable_v<Fifo<uint8_t, 6>>);
static_assert(!std::is_nothrow_move_assignable_v<Fifo<uint8_t, 6>>);

SCENARIO("FIFO: initialization")
{
	WHEN("constructing a FIFO")
	{
		const Fifo<uint8_t, 6> dut{};

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
		THEN("fifo is not full")
		{
			REQUIRE_FALSE(dut.full());
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
			THEN("fifo is not full")
			{
				REQUIRE_FALSE(dut.full());
			}
		}

		WHEN("pushing data until full")
		{
			for (size_t i = 0; i < dut.capacity(); ++i) {
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
			THEN("fifo is full")
			{
				REQUIRE(dut.full());
			}
		}

		WHEN("emplacing data until full")
		{
			for (size_t i = 0; i < dut.capacity(); ++i) {
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
			THEN("fifo is full")
			{
				REQUIRE(dut.full());
			}
		}
	}

	GIVEN("a full FIFO")
	{
		Fifo<int, 6> dut{};
		for (size_t i = 0; i < dut.capacity(); ++i) {
			REQUIRE(dut.push(i + 1));
		}
		REQUIRE(dut.size() == dut.capacity());
		REQUIRE(dut.front() == 1);
		REQUIRE(dut.full());

		WHEN("pushing additional data")
		{
			const auto result = dut.push(99);
			THEN("push fails")
			{
				REQUIRE_FALSE(result);
			}
			THEN("the oldest entry is retained")
			{
				REQUIRE(dut.front() == 1);
			}
		}

		WHEN("emplacing additional data")
		{
			const auto* result = dut.emplace(99);
			THEN("emplace fails")
			{
				REQUIRE_FALSE(result);
			}
			THEN("the oldest entry is overwritten")
			{
				REQUIRE(dut.front() == 1);
			}
		}

		WHEN("only after data is popped")
		{
			REQUIRE(dut.pop());
			REQUIRE(dut.size() == (dut.capacity() - 1));
			REQUIRE_FALSE(dut.full());

			REQUIRE(dut.pop());
			REQUIRE(dut.size() == (dut.capacity() - 2));
			REQUIRE_FALSE(dut.full());

			THEN("new data can be pushed to the queue")
			{
				REQUIRE(dut.push(dut.capacity()));
				REQUIRE(dut.size() == (dut.capacity() - 1));
				REQUIRE_FALSE(dut.full());

				REQUIRE(dut.push(dut.capacity() + 1));
				REQUIRE(dut.size() == dut.capacity());
				REQUIRE(dut.full());
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
			const auto result = dut.pop();
			THEN("pop fails")
			{
				REQUIRE_FALSE(result);
			}
			THEN("fifo remains empty")
			{
				REQUIRE(dut.size() == 0);
				REQUIRE(dut.empty());
			}
		}
	}

	GIVEN("a FIFO full of data")
	{
		Fifo<uint8_t, 6> dut{};
		for (size_t i = 0; i < dut.capacity(); ++i) {
			REQUIRE(dut.push(i + 1));
		}
		REQUIRE(dut.full());

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
			THEN("fifo is not full")
			{
				REQUIRE_FALSE(dut.full());
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
				REQUIRE(dut.size() == 0);
				REQUIRE(dut.empty());
			}
		}
	}
}

int allocations = 0;
int deallocations = 0;
int copied = 0;
int moved = 0;

SCENARIO("FIFO: store objects")
{
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
	static_assert(std::is_copy_constructible_v<Element>);
	static_assert(std::is_move_constructible_v<Element>);

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
				REQUIRE(deallocations == 3);
				REQUIRE(copied == 0);
				REQUIRE(moved == 3);
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

SCENARIO("FIFO: store objects without default constructors")
{
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
	static_assert(std::is_copy_constructible_v<Element>);
	static_assert(std::is_move_constructible_v<Element>);

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
				REQUIRE(deallocations == 2);
				REQUIRE(copied == 0);
				REQUIRE(moved == 2);
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
