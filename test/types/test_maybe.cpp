/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/types/maybe.hpp"

// #include <cstdint>
// #include <type_traits>

#include "catch2/catch_test_macros.hpp"

using namespace libecl;

namespace {
enum Method
{
	DefaultConstructed,
	CopyConstructed,
	MoveConstructed,
	ValueConstructed,
	MovedFrom,
	CopyAssigned,
	MoveAssigned,
};

class Implicit
{
public:
	Implicit()
		: m_value{42}
		, m_method{Method::DefaultConstructed}
	{
	}

	Implicit(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
	{
	}

	Implicit(Implicit const& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::CopyConstructed)
	{
	}

	Implicit& operator=(Implicit const& rhs)
	{
		m_value = rhs.m_value;
		m_method = Method::CopyAssigned;
		return *this;
	}

	Implicit(Implicit&& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::MoveConstructed)
	{
		rhs.m_method = Method::MovedFrom;
	}

	Implicit& operator=(Implicit&& rhs) noexcept
	{
		m_value = rhs.m_value;
		m_method = Method::MoveAssigned;
		rhs.m_method = Method::MovedFrom;
		return *this;
	}

	int m_value;
	Method m_method;
};

static_assert(!std::is_trivially_constructible_v<Implicit>);
static_assert(!std::is_trivially_copyable_v<Implicit>);
static_assert(!std::is_trivially_move_constructible_v<Implicit>);
static_assert(std::is_default_constructible_v<Implicit>);
static_assert(std::is_copy_constructible_v<Implicit>);
static_assert(std::is_copy_assignable_v<Implicit>);
static_assert(std::is_move_constructible_v<Implicit>);
static_assert(std::is_move_assignable_v<Implicit>);

class Explicit
{
public:
	Explicit()
		: m_value{42}
		, m_method{Method::DefaultConstructed}
	{
	}

	explicit Explicit(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
	{
	}

	Explicit(Explicit const& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::CopyConstructed)
	{
	}

	Explicit& operator=(Explicit const& rhs)
	{
		m_value = rhs.m_value;
		m_method = Method::CopyAssigned;
		return *this;
	}

	Explicit(Explicit&& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::MoveConstructed)
	{
		rhs.m_method = Method::MovedFrom;
	}

	Explicit& operator=(Explicit&& rhs) noexcept
	{
		m_value = rhs.m_value;
		m_method = Method::MoveAssigned;
		rhs.m_method = Method::MovedFrom;
		return *this;
	}

	int m_value;
	Method m_method;
};

static_assert(!std::is_trivially_constructible_v<Explicit>);
static_assert(!std::is_trivially_copyable_v<Explicit>);
static_assert(!std::is_trivially_move_constructible_v<Explicit>);
static_assert(std::is_default_constructible_v<Explicit>);
static_assert(std::is_copy_constructible_v<Explicit>);
static_assert(std::is_copy_assignable_v<Explicit>);
static_assert(std::is_move_constructible_v<Explicit>);
static_assert(std::is_move_assignable_v<Explicit>);

class NoDefault
{
public:
	NoDefault(NoDefault const& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::CopyConstructed)
	{
	}

	NoDefault& operator=(NoDefault const& rhs)
	{
		m_value = rhs.m_value;
		m_method = Method::CopyAssigned;
		return *this;
	}

	NoDefault(NoDefault&& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::MoveConstructed)
	{
		rhs.m_method = Method::MovedFrom;
	}

	NoDefault& operator=(NoDefault&& rhs) noexcept
	{
		m_value = rhs.m_value;
		m_method = Method::MoveAssigned;
		rhs.m_method = Method::MovedFrom;
		return *this;
	}

	int m_value;
	Method m_method;

private:
	NoDefault() = default;
};

static_assert(!std::is_trivially_constructible_v<NoDefault>);
static_assert(!std::is_trivially_copyable_v<NoDefault>);
static_assert(!std::is_trivially_move_constructible_v<NoDefault>);
static_assert(!std::is_default_constructible_v<NoDefault>);
static_assert(std::is_copy_constructible_v<NoDefault>);
static_assert(std::is_copy_assignable_v<NoDefault>);
static_assert(std::is_move_constructible_v<NoDefault>);
static_assert(std::is_move_assignable_v<NoDefault>);

class CopyOnly
{
public:
	CopyOnly(CopyOnly const& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::CopyConstructed)
	{
	}

	CopyOnly& operator=(CopyOnly const& rhs)
	{
		m_value = rhs.m_value;
		m_method = Method::CopyAssigned;
		return *this;
	}

	int m_value;
	Method m_method;

private:
	CopyOnly() = default;
	CopyOnly(CopyOnly&& rhs) = delete;
	CopyOnly& operator=(CopyOnly&& rhs) = delete;
};

static_assert(!std::is_trivially_constructible_v<CopyOnly>);
static_assert(!std::is_trivially_copyable_v<CopyOnly>);
static_assert(!std::is_trivially_move_constructible_v<CopyOnly>);
static_assert(!std::is_default_constructible_v<CopyOnly>);
static_assert(std::is_copy_constructible_v<CopyOnly>);
static_assert(std::is_copy_assignable_v<CopyOnly>);
static_assert(!std::is_move_constructible_v<CopyOnly>);
static_assert(!std::is_move_assignable_v<CopyOnly>);

class MoveOnly
{
public:
	MoveOnly(MoveOnly&& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::MoveConstructed)
	{
		rhs.m_method = Method::MovedFrom;
	}

	MoveOnly& operator=(MoveOnly&& rhs) noexcept
	{
		m_value = rhs.m_value;
		m_method = Method::MoveAssigned;
		rhs.m_method = Method::MovedFrom;
		return *this;
	}

	int m_value;
	Method m_method;

private:
	MoveOnly() = default;
	MoveOnly(MoveOnly const& rhs) = delete;
	MoveOnly& operator=(MoveOnly const& rhs) = delete;
};

static_assert(!std::is_trivially_constructible_v<MoveOnly>);
static_assert(!std::is_trivially_copyable_v<MoveOnly>);
static_assert(!std::is_trivially_move_constructible_v<MoveOnly>);
static_assert(!std::is_default_constructible_v<MoveOnly>);
static_assert(!std::is_copy_constructible_v<MoveOnly>);
static_assert(!std::is_copy_assignable_v<MoveOnly>);
static_assert(std::is_move_constructible_v<NoDefault>);
static_assert(std::is_move_assignable_v<NoDefault>);

class NoCopyNoMove
{
public:
	NoCopyNoMove(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
	{
	}

	virtual void foo() {}

	int m_value;
	Method m_method;

private:
	NoCopyNoMove() = default;
	NoCopyNoMove(NoCopyNoMove const& rhs) = delete;
	NoCopyNoMove& operator=(NoCopyNoMove const& rhs) = delete;
	NoCopyNoMove(NoCopyNoMove&& rhs) = delete;
	NoCopyNoMove& operator=(NoCopyNoMove&& rhs) = delete;
};

static_assert(!std::is_trivially_constructible_v<NoCopyNoMove>);
static_assert(!std::is_trivially_copyable_v<NoCopyNoMove>);
static_assert(!std::is_trivially_move_constructible_v<NoCopyNoMove>);
static_assert(!std::is_default_constructible_v<NoCopyNoMove>);
static_assert(!std::is_copy_constructible_v<NoCopyNoMove>);
static_assert(!std::is_copy_assignable_v<NoCopyNoMove>);
static_assert(!std::is_move_constructible_v<NoCopyNoMove>);
static_assert(!std::is_move_assignable_v<NoCopyNoMove>);
}  // namespace

SCENARIO("Maybe: type traits")
{
	// TODO: depends on T...
	// using T = Queue<uint8_t, 6>;

	// static_assert(std::is_constructible_v<T>);
	// static_assert(std::is_nothrow_constructible_v<T>);

	// static_assert(std::is_default_constructible_v<T>);
	// static_assert(std::is_nothrow_default_constructible_v<T>);

	// static_assert(!std::is_copy_constructible_v<T>);
	// static_assert(!std::is_copy_assignable_v<T>);

	// static_assert(!std::is_move_constructible_v<T>);
	// static_assert(!std::is_nothrow_move_constructible_v<T>);

	// static_assert(!std::is_move_assignable_v<T>);
	// static_assert(!std::is_nothrow_move_assignable_v<T>);
}

SCENARIO("Maybe: Constructors")
{
	WHEN("using the default constructor")
	{
		Maybe<int> dut{};
		Maybe<Implicit> dut_implicit{};
		Maybe<Explicit> dut_explicit{};
		Maybe<NoDefault> dut_no_default{};
		Maybe<CopyOnly> dut_copy_only{};
		Maybe<MoveOnly> dut_move_only{};
		Maybe<NoCopyNoMove> dut_no_copy_move{};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(dut.has_value());
			CHECK_FALSE(dut_implicit.has_value());
			CHECK_FALSE(dut_explicit.has_value());
			CHECK_FALSE(dut_no_default.has_value());
			CHECK_FALSE(dut_copy_only.has_value());
			CHECK_FALSE(dut_move_only.has_value());
			CHECK_FALSE(dut_no_copy_move.has_value());
		}
	}

	WHEN("explicitly constructing an empty Maybe")
	{
		Maybe<int> dut{std::nullopt};
		Maybe<Implicit> dut_implicit{std::nullopt};
		Maybe<Explicit> dut_explicit{std::nullopt};
		Maybe<NoDefault> dut_no_default{std::nullopt};
		Maybe<CopyOnly> dut_copy_only{std::nullopt};
		Maybe<MoveOnly> dut_move_only{std::nullopt};
		Maybe<NoCopyNoMove> dut_no_copy_move{std::nullopt};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(dut.has_value());
			CHECK_FALSE(dut_implicit.has_value());
			CHECK_FALSE(dut_explicit.has_value());
			CHECK_FALSE(dut_no_default.has_value());
			CHECK_FALSE(dut_copy_only.has_value());
			CHECK_FALSE(dut_move_only.has_value());
			CHECK_FALSE(dut_no_copy_move.has_value());
		}
	}

	WHEN("construction from an value")
	{
		Maybe<int> dut(102);
		Maybe<Implicit> dut_implicit(101);
		Maybe<Explicit> dut_explicit(100);

		THEN("the Maybes contains the value")
		{
			dut.match([](int val) { CHECK(val == 102); }, [](std::nullopt_t) { CHECK(false); });
			dut_implicit.match(
				[](Implicit& wrap) {
					CHECK(wrap.m_value == 101);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit.match(
				[](Explicit& wrap) {
					CHECK(wrap.m_value == 100);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("copy constructing from an empty Maybe")
	{
		Maybe<int> dut_empty{std::nullopt};
		Maybe<int> dut_copy{dut_empty};
		Maybe<Implicit> dut_implicit_empty{std::nullopt};
		Maybe<Implicit> dut_implicit_copy{dut_implicit_empty};
		Maybe<Explicit> dut_explicit_empty{std::nullopt};
		Maybe<Explicit> dut_explicit_copy{dut_explicit_empty};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(dut_copy.has_value());
			CHECK_FALSE(dut_implicit_copy.has_value());
			CHECK_FALSE(dut_explicit_copy.has_value());
		}
	}

	WHEN("copy constructing from an non-empty Maybe")
	{
		Maybe<int> dut_value{42};
		Maybe<int> dut_copy{dut_value};
		Maybe<Implicit> dut_implicit_value{64};
		Maybe<Implicit> dut_implicit_copy{dut_implicit_value};
		Maybe<Explicit> dut_explicit_value{Explicit{53}};
		Maybe<Explicit> dut_explicit_copy{dut_explicit_value};

		THEN("the Maybes contains the value")
		{
			dut_value.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut_copy.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut_implicit_value.match(
				[](Implicit& wrap) {
					CHECK(wrap.m_value == 64);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_implicit_copy.match(
				[](Implicit& wrap) {
					CHECK(wrap.m_value == 64);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit_value.match(
				[](Explicit& wrap) {
					CHECK(wrap.m_value == 53);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit_copy.match(
				[](Explicit& wrap) {
					CHECK(wrap.m_value == 53);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("copy constructing from a value")
	{
		const int value = 33;
		Maybe<int> dut{value};
		Implicit implicit_value{44};
		Maybe<Implicit> dut_implicit(implicit_value);
		Explicit explicit_value{66};
		Maybe<Explicit> dut_explicit(explicit_value);

		THEN("the Maybes contains the value")
		{
			dut.match([](int val) { CHECK(val == 33); }, [](std::nullopt_t) { CHECK(false); });
			dut_implicit.match(
				[](Implicit& wrap) {
					CHECK(wrap.m_value == 44);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit.match(
				[](Explicit& wrap) {
					CHECK(wrap.m_value == 66);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("move constructing from an empty Maybe")
	{
		Maybe<int> dut_empty{std::nullopt};
		Maybe<int> dut{std::move(dut_empty)};
		Maybe<Implicit> dut_implicit_empty{std::nullopt};
		Maybe<Implicit> dut_implicit{std::move(dut_implicit_empty)};
		Maybe<Explicit> dut_explicit_empty{std::nullopt};
		Maybe<Explicit> dut_explicit{std::move(dut_explicit_empty)};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(dut_empty.has_value());
			CHECK_FALSE(dut.has_value());
			CHECK_FALSE(dut_implicit_empty.has_value());
			CHECK_FALSE(dut_implicit.has_value());
			CHECK_FALSE(dut_explicit_empty.has_value());
			CHECK_FALSE(dut_explicit.has_value());
		}
	}

	WHEN("move constructing an non-empty Maybe")
	{
		Maybe<int> dut_value{42};
		Maybe<int> dut{std::move(dut_value)};
		Maybe<Implicit> dut_implicit_value{64};
		Maybe<Implicit> dut_implicit{std::move(dut_implicit_value)};
		Maybe<Explicit> dut_explicit_value{Explicit{53}};
		Maybe<Explicit> dut_explicit{std::move(dut_explicit_value)};

		THEN("the Maybes contain the value, the sources are empty")
		{
			CHECK(dut_value.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			dut.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			CHECK(dut_implicit_value.match([](Implicit&) { return false; }, [](std::nullopt_t) { return true; }));
			dut_implicit.match(
				[](Implicit& wrap) {
					CHECK(wrap.m_value == 64);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			CHECK(dut_explicit_value.match([](Explicit&) { return false; }, [](std::nullopt_t) { return true; }));
			dut_explicit.match(
				[](Explicit& wrap) {
					CHECK(wrap.m_value == 53);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("move constructing from a value")
	{
		Implicit implicit_value{44};
		Maybe<Implicit> dut_implicit(std::move(implicit_value));
		Explicit explicit_value{66};
		Maybe<Explicit> dut_explicit(std::move(explicit_value));

		THEN("the Maybes contain the value, the source is empty")
		{
			CHECK(implicit_value.m_method == Method::MovedFrom);
			dut_implicit.match(
				[](Implicit& wrap) {
					CHECK(wrap.m_value == 44);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			CHECK(explicit_value.m_method == Method::MovedFrom);
			dut_explicit.match(
				[](Explicit& wrap) {
					CHECK(wrap.m_value == 66);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("converting copy construction from an empty-Maybe")
	{
		Maybe<int> empty_Maybe{std::nullopt};
		Maybe<Implicit> dut_implicit{empty_Maybe};
		Maybe<Explicit> dut_explicit{empty_Maybe};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(empty_Maybe.has_value());
			CHECK_FALSE(dut_implicit.has_value());
			CHECK_FALSE(dut_explicit.has_value());
		}
	}

	WHEN("converting copy construction from an non-empty-Maybe")
	{
		Maybe<int> dut_value{42};
		Maybe<Implicit> dut_implicit{dut_value};
		Maybe<Explicit> dut_explicit{dut_value};

		THEN("the Maybes contains the value")
		{
			CHECK(dut_value.match([](int val) { return val == 42; }, [](std::nullopt_t) { return false; }));
			dut_implicit.match(
				[](Implicit& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit.match(
				[](Explicit& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("converting copy constructing from a value")
	{
		const char forty_two = '*';
		Maybe<int> dut{forty_two};
		Maybe<Implicit> dut_implicit(forty_two);
		Maybe<Explicit> dut_explicit(forty_two);

		THEN("the Maybes contains the value")
		{
			dut.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut_implicit.match(
				[](Implicit& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit.match(
				[](Explicit& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("converting move constructing from an empty Maybe")
	{
		Maybe<int> dut_implicit_empty{std::nullopt};
		Maybe<Implicit> dut_implicit{std::move(dut_implicit_empty)};
		Maybe<int> dut_explicit_empty{std::nullopt};
		Maybe<Explicit> dut_explicit{std::move(dut_explicit_empty)};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(dut_implicit_empty.has_value());
			CHECK_FALSE(dut_implicit.has_value());
			CHECK_FALSE(dut_explicit_empty.has_value());
			CHECK_FALSE(dut_explicit.has_value());
		}
	}

	WHEN("converting move constructing an non-empty Maybe")
	{
		Maybe<int> dut_implicit_value{33};
		Maybe<Implicit> dut_implicit1{std::move(dut_implicit_value)};
		Maybe<int> dut_explicit_value{42};
		Maybe<Explicit> dut_explicit1{std::move(dut_explicit_value)};

		const int implicit_value = 34;
		Maybe<Implicit> dut_implicit2{std::move(implicit_value)};
		const int explicit_value = 43;
		Maybe<Explicit> dut_explicit2{std::move(explicit_value)};

		THEN("the Maybes contain the value, the sources are empty")
		{
			CHECK(dut_implicit_value.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			dut_implicit1.match(
				[](Implicit& wrap) {
					CHECK(wrap.m_value == 33);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			CHECK(dut_explicit_value.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			dut_explicit1.match(
				[](Explicit& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_implicit2.match(
				[](Implicit& wrap) {
					CHECK(wrap.m_value == 34);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit2.match(
				[](Explicit& wrap) {
					CHECK(wrap.m_value == 43);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("converting move constructing from a value")
	{
		Implicit implicit_value{44};
		Maybe<Implicit> dut_implicit(std::move(implicit_value));
		Explicit explicit_value{66};
		Maybe<Explicit> dut_explicit(std::move(explicit_value));

		THEN("the Maybes contain the value, the source is empty")
		{
			CHECK(implicit_value.m_method == Method::MovedFrom);
			dut_implicit.match(
				[](Implicit& wrap) {
					CHECK(wrap.m_value == 44);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			CHECK(explicit_value.m_method == Method::MovedFrom);
			dut_explicit.match(
				[](Explicit& wrap) {
					CHECK(wrap.m_value == 66);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("in-place construction")
	{
		using pair_t = std::pair<char, int>;
		Maybe<pair_t> dut1(std::in_place, '4', 2);

		const char value = '4';
		const int number = 2;
		Maybe<const pair_t> dut2(std::in_place, value, number);

		THEN("the Maybe contains the value")
		{
			dut1.match(
				[](pair_t& pair) {
					CHECK(pair.first == '4');
					CHECK(pair.second == 2);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut2.match(
				[](pair_t const& pair) {
					CHECK(pair.first == '4');
					CHECK(pair.second == 2);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("in-place construction with initializer list")
	{
		using vec_t = std::vector<int>;
		Maybe<vec_t> dut(std::in_place, {1, 2, 3, 4, 5});

		THEN("the Maybe contains the value")
		{
			dut.match(
				[](vec_t& vec) {
					REQUIRE(vec.size() == 5);
					CHECK(vec[0] == 1);
					CHECK(vec[1] == 2);
					CHECK(vec[2] == 3);
					CHECK(vec[3] == 4);
					CHECK(vec[4] == 5);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}
}

SCENARIO("Maybe: Assignment")
{
	GIVEN("nullopt assignment")
	{
		Maybe<int> dut_empty;
		Maybe<int> dut_value(42);

		WHEN("assigning nullopt")
		{
			dut_empty = std::nullopt;
			dut_value = std::nullopt;

			THEN("the Maybes are empty")
			{
				CHECK_FALSE(dut_empty.has_value());
				CHECK_FALSE(dut_value.has_value());
			}
		}
	}

	GIVEN("copy assignment from another Maybe")
	{
		Maybe<Implicit> dut_empty_lhs;
		Maybe<Implicit> dut_empty_rhs;
		Maybe<Implicit> dut_value_lhs(42);
		Maybe<Implicit> dut_value_rhs(33);

		WHEN("lhs has a value, rhs has a value")
		{
			dut_value_lhs = dut_value_rhs;

			THEN("the Maybes hold the same value")
			{
				dut_value_lhs.match(
					[](Implicit& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::CopyAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });
				dut_value_rhs.match(
					[](Implicit& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
			}
		}

		WHEN("lhs has a value, rhs is empty")
		{
			dut_value_lhs = dut_empty_rhs;

			THEN("the Maybes are empty")
			{
				CHECK_FALSE(dut_value_lhs.has_value());
				CHECK_FALSE(dut_empty_rhs.has_value());
			}
		}

		WHEN("lhs is empty, rhs has a value")
		{
			dut_empty_lhs = dut_value_rhs;

			THEN("the Maybes hold the same value")
			{
				dut_empty_lhs.match(
					[](Implicit& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::CopyAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });
				dut_value_rhs.match(
					[](Implicit& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
			}
		}

		WHEN("lhs is empty, rhs is empty")
		{
			dut_empty_lhs = dut_empty_rhs;

			THEN("the Maybes are empty")
			{
				CHECK_FALSE(dut_empty_lhs.has_value());
				CHECK_FALSE(dut_empty_rhs.has_value());
			}
		}
	}

	GIVEN("copy assignment from a value")
	{
		Maybe<int> dut1;
		Maybe<int> dut2;

		dut1 = 42;
		const int value = 101;
		dut2 = value;

		THEN("the Maybes contains the value")
		{
			dut1.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut2.match([](int val) { CHECK(val == 101); }, [](std::nullopt_t) { CHECK(false); });
		}
	}

	GIVEN("move assignment from another Maybe")
	{
		Maybe<Implicit> dut_empty_lhs;
		Maybe<Implicit> dut_empty_rhs;
		Maybe<Implicit> dut_value_lhs(42);
		Maybe<Implicit> dut_value_rhs(33);

		WHEN("lhs has a value, rhs has a value")
		{
			dut_value_lhs = std::move(dut_value_rhs);

			THEN("the Maybes hold the same value")
			{
				dut_value_lhs.match(
					[](Implicit& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::MoveAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK_FALSE(dut_value_rhs.has_value());
			}
		}

		WHEN("lhs has a value, rhs is empty")
		{
			dut_value_lhs = std::move(dut_empty_rhs);

			THEN("the Maybes are empty")
			{
				CHECK_FALSE(dut_value_lhs.has_value());
				CHECK_FALSE(dut_empty_rhs.has_value());
			}
		}

		WHEN("lhs is empty, rhs has a value")
		{
			dut_empty_lhs = std::move(dut_value_rhs);

			THEN("the Maybes hold the same value")
			{
				dut_empty_lhs.match(
					[](Implicit& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::MoveAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK_FALSE(dut_value_rhs.has_value());
			}
		}

		WHEN("lhs is empty, rhs is empty")
		{
			dut_empty_lhs = std::move(dut_empty_rhs);

			THEN("the Maybes are empty")
			{
				CHECK_FALSE(dut_empty_lhs.has_value());
				CHECK_FALSE(dut_empty_rhs.has_value());
			}
		}
	}

	GIVEN("move assignment from a value")
	{
		Implicit value = 101;
		Maybe<Implicit> dut = std::move(value);

		THEN("the Maybes contains the value")
		{
			dut.match(
				[](Implicit& wrap) {
					CHECK(wrap.m_value == 101);
					CHECK(wrap.m_method == Method::MoveAssigned);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	GIVEN("emplace")
	{
		CHECK(false);
	}

	GIVEN("emplace")
	{
		CHECK(false);
	}
}

SCENARIO("Maybe: swap")
{
	CHECK(false);
}

SCENARIO("Maybe: observe")
{
	GIVEN("an empty Maybe")
	{
		Maybe<int> dut;

		WHEN("calling match")
		{
			THEN("returns nullopt")
			{
				CHECK(dut.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			}
		}

		WHEN("calling value_or")
		{
			THEN("returns the fallback value")
			{
				CHECK(dut.value_or(6) == 6);
			}
		}
	}

	GIVEN("an Maybe with a value")
	{
		Maybe<int> dut = 44;

		WHEN("calling match")
		{
			THEN("the value can be retrieved")
			{
				CHECK(dut.match([](int value) { return value == 44; }, [](std::nullopt_t) { return true; }));
			}
		}

		WHEN("calling value_or")
		{
			THEN("the value can be retrieved")
			{
				CHECK(dut.value_or(6) == 44);
			}
		}
	}
}

SCENARIO("Maybe: modifiers")
{
	CHECK(false);
}

SCENARIO("Maybe: invalid types")
{
	// Maybe<std::in_place_t> dut1{};
	// Maybe<void> dut2{};
	// Maybe<std::nullopt_t> dut3{};
}

//!\todo has_value / operator bool
//!\todo reset?
