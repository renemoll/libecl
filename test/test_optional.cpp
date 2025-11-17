/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/utilities/optional.hpp"

// #include <cstdint>
// #include <type_traits>

#include "catch2/catch_test_macros.hpp"

using namespace libecl;

SCENARIO("Optional: type traits")
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

SCENARIO("Optional: construction")
{
	enum Method
	{
		DefaultConstructed,	 // done
		CopyConstructed,	 // done
		MoveConstructed,	 // done
		ValueConstructed,	 // done
		MovedFrom,			 // done
		CopyAssignment,		 // todo
		MoveAssignment,		 // todo
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
			m_method = Method::CopyAssignment;
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
			m_method = Method::MoveAssignment;
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
			m_method = Method::CopyAssignment;
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
			m_method = Method::MoveAssignment;
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
			m_method = Method::CopyAssignment;
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
			m_method = Method::MoveAssignment;
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
			m_method = Method::CopyAssignment;
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
			m_method = Method::MoveAssignment;
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

		virtual void foo();

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

	WHEN("using the default constructor")
	{
		Optional<int> dut{};
		Optional<Implicit> dut_implicit{};
		Optional<Explicit> dut_explicit{};
		Optional<NoDefault> dut_no_default{};
		Optional<CopyOnly> dut_copy_only{};
		Optional<MoveOnly> dut_move_only{};
		Optional<NoCopyNoMove> dut_no_copy_move{};

		THEN("the Optional is empty")
		{
			CHECK(dut.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_implicit.match([](Implicit&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_explicit.match([](Explicit&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_no_default.match([](NoDefault&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_copy_only.match([](CopyOnly&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_move_only.match([](MoveOnly&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_no_copy_move.match([](NoCopyNoMove&) { return false; }, [](std::nullopt_t) { return true; }));
		}
	}

	WHEN("explicitly constructing an empty Optional")
	{
		Optional<int> dut{std::nullopt};
		Optional<Implicit> dut_implicit{std::nullopt};
		Optional<Explicit> dut_explicit{std::nullopt};
		Optional<NoDefault> dut_no_default{std::nullopt};
		Optional<CopyOnly> dut_copy_only{std::nullopt};
		Optional<MoveOnly> dut_move_only{std::nullopt};
		Optional<NoCopyNoMove> dut_no_copy_move{std::nullopt};

		THEN("the Optional is empty")
		{
			CHECK(dut.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_implicit.match([](Implicit&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_explicit.match([](Explicit&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_no_default.match([](NoDefault&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_copy_only.match([](CopyOnly&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_move_only.match([](MoveOnly&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_no_copy_move.match([](NoCopyNoMove&) { return false; }, [](std::nullopt_t) { return true; }));
		}
	}

	WHEN("construction from an value")
	{
		Optional<int> dut1(102);
		Optional<int> dut2 = 42;

		const int value = 33;
		Optional<int> dut3{value};
		Optional<int> dut4 = value;

		const char forty_two = '*';
		Optional<int> dut5 = forty_two;
		Optional<int> dut6{'*'};

		THEN("the Optional contains the value")
		{
			dut1.match([](int val) { CHECK(val == 102); }, [](std::nullopt_t) { CHECK(false); });
			dut2.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut3.match([](int val) { CHECK(val == 33); }, [](std::nullopt_t) { CHECK(false); });
			dut4.match([](int val) { CHECK(val == 33); }, [](std::nullopt_t) { CHECK(false); });
			dut5.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut6.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("copy constructing an empty Optional")
	{
		Optional<int> dut_empty{std::nullopt};
		Optional<int> dut_copy{dut_empty};
		Optional<Implicit> dut_implicit_empty{std::nullopt};
		Optional<Implicit> dut_implicit_copy{dut_implicit_empty};
		Optional<Explicit> dut_explicit_empty{std::nullopt};
		Optional<Explicit> dut_explicit_copy{dut_explicit_empty};

		THEN("the Optional is empty")
		{
			CHECK(dut_copy.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_implicit_copy.match([](Implicit&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_explicit_copy.match([](Explicit&) { return false; }, [](std::nullopt_t) { return true; }));
		}
	}

	WHEN("copy constructing an non-empty Optional")
	{
		Optional<int> dut_value{42};
		Optional<int> dut_copy{dut_value};
		Optional<Implicit> dut_implicit_value{64};
		Optional<Implicit> dut_implicit_copy{dut_implicit_value};
		Optional<Explicit> dut_explicit_value{Explicit{53}};
		Optional<Explicit> dut_explicit_copy{dut_explicit_value};

		THEN("the Optional contains the value")
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

	WHEN("move constructing an empty Optional")
	{
		Optional<int> dut_empty{std::nullopt};
		Optional<int> dut{std::move(dut_empty)};
		Optional<Implicit> dut_implicit_empty{std::nullopt};
		Optional<Implicit> dut_implicit{std::move(dut_implicit_empty)};
		Optional<Explicit> dut_explicit_empty{std::nullopt};
		Optional<Explicit> dut_explicit{std::move(dut_explicit_empty)};

		THEN("the Optional is empty")
		{
			CHECK(dut_empty.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_implicit_empty.match([](Implicit&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_implicit.match([](Implicit&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_explicit_empty.match([](Explicit&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_explicit.match([](Explicit&) { return false; }, [](std::nullopt_t) { return true; }));
		}
	}

	WHEN("move constructing an non-empty Optional")
	{
		Optional<int> dut_value{42};
		Optional<int> dut{std::move(dut_value)};
		Optional<Implicit> dut_implicit_value{64};
		Optional<Implicit> dut_implicit{std::move(dut_implicit_value)};
		Optional<Explicit> dut_explicit_value{Explicit{53}};
		Optional<Explicit> dut_explicit{std::move(dut_explicit_value)};

		THEN("the Optional contains the value")
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

	WHEN("converting copy construction from an empty-Optional")
	{
		Optional<int> empty_optional{std::nullopt};
		Optional<Implicit> dut_implicit{empty_optional};
		Optional<Explicit> dut_explicit{empty_optional};

		THEN("the Optional is empty")
		{
			CHECK(empty_optional.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_implicit.match([](Implicit&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_explicit.match([](Explicit&) { return false; }, [](std::nullopt_t) { return true; }));
		}
	}

	WHEN("converting copy construction from an non-empty-Optional")
	{
		Optional<int> dut_value{42};
		Optional<Implicit> dut_implicit{dut_value};
		Optional<Explicit> dut_explicit{dut_value};

		THEN("the Optional contains the value")
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

	WHEN("converting move constructing an empty Optional")
	{
		Optional<int> dut_implicit_empty{std::nullopt};
		Optional<Implicit> dut_implicit{std::move(dut_implicit_empty)};
		Optional<int> dut_explicit_empty{std::nullopt};
		Optional<Explicit> dut_explicit{std::move(dut_explicit_empty)};

		THEN("the Optional is empty")
		{
			CHECK(dut_implicit_empty.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_implicit.match([](Implicit&) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_explicit_empty.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_explicit.match([](Explicit&) { return false; }, [](std::nullopt_t) { return true; }));
		}
	}

	WHEN("converting move constructing an non-empty Optional")
	{
		Optional<int> dut_implicit_value{33};
		Optional<Implicit> dut_implicit1{std::move(dut_implicit_value)};
		Optional<int> dut_explicit_value{42};
		Optional<Explicit> dut_explicit1{std::move(dut_explicit_value)};

		const int implicit_value = 34;
		Optional<Implicit> dut_implicit2{std::move(implicit_value)};
		const int explicit_value = 43;
		Optional<Explicit> dut_explicit2{std::move(explicit_value)};

		THEN("the Optional contains the value")
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

	WHEN("in-place construction")
	{
		using pair_t = std::pair<char, int>;
		Optional<pair_t> dut1(std::in_place, '4', 2);

		const char value = '4';
		const int number = 2;
		Optional<const pair_t> dut2(std::in_place, value, number);

		THEN("the Optional contains the value")
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
		Optional<vec_t> dut(std::in_place, {1, 2, 3, 4, 5});

		THEN("the Optional contains the value")
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

SCENARIO("Optional: accessors")
{
	GIVEN("an empty Optional")
	{
		Optional<int> dut;

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

	GIVEN("an Optional with a value")
	{
		Optional<int> dut = 44;

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

SCENARIO("Optional: invalid types")
{
	// Optional<std::in_place_t> dut1{};
	// Optional<void> dut2{};
	// Optional<std::nullopt_t> dut3{};
}

//!\todo has_value / operator bool
//!\todo reset?
