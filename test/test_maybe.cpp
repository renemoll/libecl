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
#include <string>

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

// class Implicit
// {
// public:
// 	Implicit()
// 		: m_value{42}
// 		, m_method{Method::DefaultConstructed}
// 	{
// 	}

// 	Implicit(int value)
// 		: m_value{value}
// 		, m_method{Method::ValueConstructed}
// 	{
// 	}

// 	Implicit(Implicit const& rhs)
// 		: m_value(rhs.m_value)
// 		, m_method(Method::CopyConstructed)
// 	{
// 	}

// 	Implicit& operator=(Implicit const& rhs)
// 	{
// 		m_value = rhs.m_value;
// 		m_method = Method::CopyAssigned;
// 		return *this;
// 	}

// 	Implicit(Implicit&& rhs) noexcept
// 		: m_value(rhs.m_value)
// 		, m_method(Method::MoveConstructed)
// 	{
// 		rhs.m_method = Method::MovedFrom;
// 	}

// 	Implicit& operator=(Implicit&& rhs) noexcept
// 	{
// 		m_value = rhs.m_value;
// 		m_method = Method::MoveAssigned;
// 		rhs.m_method = Method::MovedFrom;
// 		return *this;
// 	}

// 	int m_value;
// 	Method m_method;
// };

// static_assert(!std::is_trivially_constructible_v<Implicit>);
// static_assert(!std::is_trivially_copyable_v<Implicit>);
// static_assert(!std::is_trivially_move_constructible_v<Implicit>);
// static_assert(std::is_default_constructible_v<Implicit>);
// static_assert(std::is_copy_constructible_v<Implicit>);
// static_assert(std::is_copy_assignable_v<Implicit>);
// static_assert(std::is_move_constructible_v<Implicit>);
// static_assert(std::is_move_assignable_v<Implicit>);

class DefaultConstructible
{
public:
	DefaultConstructible()
		: m_value{42}
		, m_method{Method::DefaultConstructed}
		, m_string{"DefaultConstructible"}
	{
	}

	explicit DefaultConstructible(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
		, m_string{"DefaultConstructible"}
	{
	}

	DefaultConstructible(DefaultConstructible const& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::CopyConstructed)
		, m_string(rhs.m_string)
	{
	}

	DefaultConstructible& operator=(DefaultConstructible const& rhs)
	{
		m_value = rhs.m_value;
		m_method = Method::CopyAssigned;
		m_string = rhs.m_string;
		return *this;
	}

	DefaultConstructible(DefaultConstructible&& rhs) noexcept
		: m_value(rhs.m_value)
		, m_method(Method::MoveConstructed)
		, m_string(std::move(rhs.m_string))
	{
		rhs.m_method = Method::MovedFrom;
	}

	DefaultConstructible& operator=(DefaultConstructible&& rhs) noexcept
	{
		m_value = rhs.m_value;
		m_method = Method::MoveAssigned;
		m_string = std::move(rhs.m_string);
		rhs.m_method = Method::MovedFrom;
		return *this;
	}

	int m_value;
	Method m_method;
	std::string m_string;
};

static_assert(!std::is_trivially_constructible_v<DefaultConstructible>);
static_assert(!std::is_trivially_copyable_v<DefaultConstructible>);
static_assert(!std::is_trivially_move_constructible_v<DefaultConstructible>);
static_assert(std::is_default_constructible_v<DefaultConstructible>);
static_assert(std::is_copy_constructible_v<DefaultConstructible>);
static_assert(std::is_copy_assignable_v<DefaultConstructible>);
static_assert(std::is_move_constructible_v<DefaultConstructible>);
static_assert(std::is_move_assignable_v<DefaultConstructible>);

class NonDefaultConstructible
{
public:
	explicit NonDefaultConstructible(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
		, m_string{"NonDefaultConstructible"}
	{
	}

	NonDefaultConstructible(NonDefaultConstructible const& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::CopyConstructed)
		, m_string(rhs.m_string)
	{
	}

	NonDefaultConstructible& operator=(NonDefaultConstructible const& rhs)
	{
		m_value = rhs.m_value;
		m_method = Method::CopyAssigned;
		m_string = rhs.m_string;
		return *this;
	}

	NonDefaultConstructible(NonDefaultConstructible&& rhs) noexcept
		: m_value(rhs.m_value)
		, m_method(Method::MoveConstructed)
		, m_string(std::move(rhs.m_string))
	{
		rhs.m_method = Method::MovedFrom;
	}

	NonDefaultConstructible& operator=(NonDefaultConstructible&& rhs) noexcept
	{
		m_value = rhs.m_value;
		m_method = Method::MoveAssigned;
		m_string = std::move(rhs.m_string);
		rhs.m_method = Method::MovedFrom;
		return *this;
	}

	int m_value;
	Method m_method;
	std::string m_string;
};

static_assert(!std::is_trivially_constructible_v<NonDefaultConstructible>);
static_assert(!std::is_trivially_copyable_v<NonDefaultConstructible>);
static_assert(!std::is_trivially_move_constructible_v<NonDefaultConstructible>);
static_assert(!std::is_default_constructible_v<NonDefaultConstructible>);
static_assert(std::is_copy_constructible_v<NonDefaultConstructible>);
static_assert(std::is_copy_assignable_v<NonDefaultConstructible>);
static_assert(std::is_move_constructible_v<NonDefaultConstructible>);
static_assert(std::is_move_assignable_v<NonDefaultConstructible>);

class NonCopyable
{
public:
	NonCopyable()
		: m_value{42}
		, m_method{Method::DefaultConstructed}
		, m_string{"NonCopyable"}
	{
	}

	explicit NonCopyable(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
		, m_string{"NonCopyable"}
	{
	}

	NonCopyable(NonCopyable const& rhs) = delete;
	NonCopyable& operator=(NonCopyable const& rhs) = delete;

	NonCopyable(NonCopyable&& rhs) noexcept
		: m_value(rhs.m_value)
		, m_method(Method::MoveConstructed)
		, m_string(std::move(rhs.m_string))
	{
		rhs.m_method = Method::MovedFrom;
	}

	NonCopyable& operator=(NonCopyable&& rhs) noexcept
	{
		m_value = rhs.m_value;
		m_method = Method::MoveAssigned;
		m_string = std::move(rhs.m_string);
		rhs.m_method = Method::MovedFrom;
		return *this;
	}

	int m_value;
	Method m_method;
	std::string m_string;
};

static_assert(!std::is_trivially_constructible_v<NonCopyable>);
static_assert(!std::is_trivially_copyable_v<NonCopyable>);
static_assert(!std::is_trivially_move_constructible_v<NonCopyable>);
static_assert(std::is_default_constructible_v<NonCopyable>);
static_assert(!std::is_copy_constructible_v<NonCopyable>);
static_assert(!std::is_copy_assignable_v<NonCopyable>);
static_assert(std::is_move_constructible_v<NonCopyable>);
static_assert(std::is_move_assignable_v<NonCopyable>);

class NonMovable
{
public:
	NonMovable()
		: m_value{42}
		, m_method{Method::DefaultConstructed}
		, m_string{"NonMovable"}
	{
	}

	explicit NonMovable(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
		, m_string{"NonMovable"}
	{
	}

	NonMovable(NonMovable const& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::CopyConstructed)
		, m_string(rhs.m_string)
	{
	}

	NonMovable& operator=(NonMovable const& rhs)
	{
		m_value = rhs.m_value;
		m_method = Method::CopyAssigned;
		m_string = rhs.m_string;
		return *this;
	}

	NonMovable(NonMovable&& rhs) = delete;
	NonMovable& operator=(NonMovable&& rhs) = delete;

	int m_value;
	Method m_method;
	std::string m_string;
};

static_assert(!std::is_trivially_constructible_v<NonMovable>);
static_assert(!std::is_trivially_copyable_v<NonMovable>);
static_assert(!std::is_trivially_move_constructible_v<NonMovable>);
static_assert(std::is_default_constructible_v<NonMovable>);
static_assert(std::is_copy_constructible_v<NonMovable>);
static_assert(std::is_copy_assignable_v<NonMovable>);
static_assert(!std::is_move_constructible_v<NonMovable>);
static_assert(!std::is_move_assignable_v<NonMovable>);

class NonCopyableNonMovable
{
public:
	NonCopyableNonMovable()
		: m_value{42}
		, m_method{Method::DefaultConstructed}
		, m_string{"NonCopyableNonMovable"}
	{
	}

	explicit NonCopyableNonMovable(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
		, m_string{"NonCopyableNonMovable"}
	{
	}

	NonCopyableNonMovable(NonCopyableNonMovable const& rhs) = delete;
	NonCopyableNonMovable& operator=(NonCopyableNonMovable const& rhs) = delete;
	NonCopyableNonMovable(NonCopyableNonMovable&& rhs) = delete;
	NonCopyableNonMovable& operator=(NonCopyableNonMovable&& rhs) = delete;

	int m_value;
	Method m_method;
	std::string m_string;
};

static_assert(!std::is_trivially_constructible_v<NonCopyableNonMovable>);
static_assert(!std::is_trivially_copyable_v<NonCopyableNonMovable>);
static_assert(!std::is_trivially_move_constructible_v<NonCopyableNonMovable>);
static_assert(std::is_default_constructible_v<NonCopyableNonMovable>);
static_assert(!std::is_copy_constructible_v<NonCopyableNonMovable>);
static_assert(!std::is_copy_assignable_v<NonCopyableNonMovable>);
static_assert(!std::is_move_constructible_v<NonCopyableNonMovable>);
static_assert(!std::is_move_assignable_v<NonCopyableNonMovable>);
}  // namespace

SCENARIO("Maybe: type traits")
{
	CHECK(std::is_copy_constructible_v<DefaultConstructible>);
	CHECK(std::is_copy_assignable_v<DefaultConstructible>);
	CHECK(std::is_move_constructible_v<DefaultConstructible>);
	CHECK(std::is_move_assignable_v<DefaultConstructible>);

	CHECK(std::is_copy_constructible_v<Maybe<DefaultConstructible>>);
	CHECK(std::is_copy_assignable_v<Maybe<DefaultConstructible>>);
	CHECK(std::is_move_constructible_v<Maybe<DefaultConstructible>>);
	CHECK(std::is_move_assignable_v<Maybe<DefaultConstructible>>);

	CHECK(std::is_copy_constructible_v<NonDefaultConstructible>);
	CHECK(std::is_copy_assignable_v<NonDefaultConstructible>);
	CHECK(std::is_move_constructible_v<NonDefaultConstructible>);
	CHECK(std::is_move_assignable_v<NonDefaultConstructible>);

	CHECK(std::is_copy_constructible_v<Maybe<NonDefaultConstructible>>);
	CHECK(std::is_copy_assignable_v<Maybe<NonDefaultConstructible>>);
	CHECK(std::is_move_constructible_v<Maybe<NonDefaultConstructible>>);
	CHECK(std::is_move_assignable_v<Maybe<NonDefaultConstructible>>);

	CHECK(!std::is_copy_constructible_v<NonCopyable>);
	CHECK(!std::is_copy_assignable_v<NonCopyable>);
	CHECK(std::is_move_constructible_v<NonCopyable>);
	CHECK(std::is_move_assignable_v<NonCopyable>);

	CHECK(!std::is_copy_constructible_v<Maybe<NonCopyable>>);
	CHECK(!std::is_copy_assignable_v<Maybe<NonCopyable>>);
	CHECK(std::is_move_constructible_v<Maybe<NonCopyable>>);
	CHECK(std::is_move_assignable_v<Maybe<NonCopyable>>);

	CHECK(std::is_copy_constructible_v<NonMovable>);
	CHECK(std::is_copy_assignable_v<NonMovable>);
	CHECK(!std::is_move_constructible_v<NonMovable>);
	CHECK(!std::is_move_assignable_v<NonMovable>);

	CHECK(std::is_copy_constructible_v<Maybe<NonMovable>>);
	CHECK(std::is_copy_assignable_v<Maybe<NonMovable>>);
	CHECK(!std::is_move_constructible_v<Maybe<NonMovable>>);
	CHECK(!std::is_move_assignable_v<Maybe<NonMovable>>);

	CHECK(!std::is_copy_constructible_v<NonCopyableNonMovable>);
	CHECK(!std::is_copy_assignable_v<NonCopyableNonMovable>);
	CHECK(!std::is_move_constructible_v<NonCopyableNonMovable>);
	CHECK(!std::is_move_assignable_v<NonCopyableNonMovable>);

	CHECK(!std::is_copy_constructible_v<Maybe<NonCopyableNonMovable>>);
	CHECK(!std::is_copy_assignable_v<Maybe<NonCopyableNonMovable>>);
	CHECK(!std::is_move_constructible_v<Maybe<NonCopyableNonMovable>>);
	CHECK(!std::is_move_assignable_v<Maybe<NonCopyableNonMovable>>);
}

SCENARIO("Maybe: constructors")
{
	WHEN("using the default constructor")
	{
		Maybe<int> dut{};
		Maybe<DefaultConstructible> dut_default{};
		Maybe<NonDefaultConstructible> dut_no_default{};
		Maybe<NonCopyable> dut_no_copy{};
		Maybe<NonMovable> dut_no_move{};
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move{};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(dut.has_value());
			CHECK_FALSE(dut_default.has_value());
			CHECK_FALSE(dut_no_default.has_value());
			CHECK_FALSE(dut_no_copy.has_value());
			CHECK_FALSE(dut_no_move.has_value());
			CHECK_FALSE(dut_no_copy_no_move.has_value());
		}
	}

	WHEN("explicitly constructing an empty Maybe")
	{
		Maybe<int> dut{std::nullopt};
		Maybe<DefaultConstructible> dut_default{std::nullopt};
		Maybe<NonDefaultConstructible> dut_no_default{std::nullopt};
		Maybe<NonCopyable> dut_no_copy{std::nullopt};
		Maybe<NonMovable> dut_no_move{std::nullopt};
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move{std::nullopt};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(dut.has_value());
			CHECK_FALSE(dut_default.has_value());
			CHECK_FALSE(dut_no_default.has_value());
			CHECK_FALSE(dut_no_copy.has_value());
			CHECK_FALSE(dut_no_move.has_value());
			CHECK_FALSE(dut_no_copy_no_move.has_value());
		}
	}

	WHEN("construction from an value")
	{
		Maybe<int> dut_primitive(102);
		Maybe<DefaultConstructible> dut_default(DefaultConstructible(100));
		const NonDefaultConstructible no_default_value(200);
		const Maybe<NonDefaultConstructible> dut_no_default(no_default_value);

		THEN("the Maybes contains the value")
		{
			dut_primitive.match([](int val) { CHECK(val == 102); }, [](std::nullopt_t) { CHECK(false); });
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 100);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_default.match(
				[](const NonDefaultConstructible& wrap) {
					CHECK(wrap.m_value == 200);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("copy constructing from an empty Maybe")
	{
		Maybe<int> dut_empty{std::nullopt};
		Maybe<int> dut_copy{dut_empty};
		Maybe<DefaultConstructible> dut_default_empty{std::nullopt};
		Maybe<DefaultConstructible> dut_default_copy{dut_default_empty};
		const Maybe<NonDefaultConstructible> dut_no_default_empty{std::nullopt};
		const Maybe<NonDefaultConstructible> dut_no_default_copy{dut_no_default_empty};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(dut_copy.has_value());
			CHECK_FALSE(dut_default_copy.has_value());
			CHECK_FALSE(dut_default_copy.has_value());
			CHECK_FALSE(dut_no_default_copy.has_value());
		}
	}

	WHEN("copy constructing from an non-empty Maybe")
	{
		Maybe<int> dut_value{42};
		Maybe<int> dut_copy{dut_value};
		Maybe<DefaultConstructible> dut_default{DefaultConstructible{53}};
		Maybe<DefaultConstructible> dut_default_copy{dut_default};
		const Maybe<NonDefaultConstructible> dut_no_default{NonDefaultConstructible{64}};
		const Maybe<NonDefaultConstructible> dut_no_default_copy{dut_no_default};

		THEN("the Maybes contains the value")
		{
			dut_value.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut_copy.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 53);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_default_copy.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 53);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_default.match(
				[](const NonDefaultConstructible& wrap) {
					CHECK(wrap.m_value == 64);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("copy constructing from a value")
	{
		const int value = 33;
		Maybe<int> dut{value};
		DefaultConstructible default_value{66};
		Maybe<DefaultConstructible> dut_default(default_value);
		const NonDefaultConstructible no_default_value{44};
		const Maybe<NonDefaultConstructible> dut_no_default(no_default_value);

		THEN("the Maybes contains the value")
		{
			dut.match([](int val) { CHECK(val == 33); }, [](std::nullopt_t) { CHECK(false); });
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 66);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_default.match(
				[](const NonDefaultConstructible& wrap) {
					CHECK(wrap.m_value == 44);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("move constructing from an empty Maybe")
	{
		Maybe<int> dut_empty{std::nullopt};
		Maybe<int> dut{std::move(dut_empty)};
		const Maybe<DefaultConstructible> dut_default_empty{std::nullopt};
		const Maybe<DefaultConstructible> dut_default{std::move(dut_default_empty)};
		Maybe<NonDefaultConstructible> dut_no_default_empty{std::nullopt};
		Maybe<NonDefaultConstructible> dut_no_default{std::move(dut_no_default_empty)};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(dut_empty.has_value());
			CHECK_FALSE(dut.has_value());
			CHECK_FALSE(dut_default_empty.has_value());
			CHECK_FALSE(dut_default.has_value());
			CHECK_FALSE(dut_no_default_empty.has_value());
			CHECK_FALSE(dut_no_default.has_value());
		}
	}

	WHEN("move constructing from a non-empty Maybe")
	{
		Maybe<int> dut_value{42};
		Maybe<int> dut{std::move(dut_value)};
		Maybe<DefaultConstructible> dut_default_value{DefaultConstructible{53}};
		Maybe<DefaultConstructible> dut_default{std::move(dut_default_value)};
		Maybe<NonDefaultConstructible> dut_no_default_value{NonDefaultConstructible{64}};
		const Maybe<NonDefaultConstructible> dut_no_default{std::move(dut_no_default_value)};

		THEN("the Maybes contain the value, the sources are empty")
		{
			CHECK(dut_value.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			CHECK(dut_default_value.match([](const DefaultConstructible&) { return false; },
										  [](std::nullopt_t) { return true; }));
			CHECK(dut_no_default_value.match([](const NonDefaultConstructible&) { return false; },
											 [](std::nullopt_t) { return true; }));

			dut.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 53);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_default.match(
				[](const NonDefaultConstructible& wrap) {
					CHECK(wrap.m_value == 64);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("move constructing from a value")
	{
		int temp{44};
		Maybe<int> dut_primitive(std::move(temp));
		DefaultConstructible default_value{66};
		Maybe<DefaultConstructible> dut_default(std::move(default_value));
		NonDefaultConstructible no_default_value{77};
		const Maybe<NonDefaultConstructible> dut_no_default(std::move(no_default_value));

		THEN("the Maybes contain the value, the source is empty")
		{
			CHECK(temp == 44);
			dut_primitive.match(
				[](int val) {
					CHECK(val == 44);
					CHECK(val == 44);
				},
				[](std::nullopt_t) { CHECK(false); });

			CHECK(default_value.m_method == Method::MovedFrom);
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 66);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });

			CHECK(no_default_value.m_method == Method::MovedFrom);
			dut_no_default.match(
				[](const NonDefaultConstructible& wrap) {
					CHECK(wrap.m_value == 77);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("converting copy construction from an empty-Maybe")
	{
		Maybe<int> empty_Maybe{std::nullopt};
		Maybe<DefaultConstructible> dut_default{empty_Maybe};
		const Maybe<NonDefaultConstructible> dut_no_default{empty_Maybe};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(empty_Maybe.has_value());
			CHECK_FALSE(dut_default.has_value());
			CHECK_FALSE(dut_no_default.has_value());
		}
	}

	WHEN("converting copy construction from an non-empty-Maybe")
	{
		Maybe<int> dut_value{42};
		Maybe<DefaultConstructible> dut_default{dut_value};
		const Maybe<NonDefaultConstructible> dut_no_default{dut_value};

		THEN("the Maybes contains the value")
		{
			CHECK(dut_value.match([](int val) { return val == 42; }, [](std::nullopt_t) { return false; }));
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 42);
					// Note: copy constructor creates a copy which is moved into the storage
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_default.match(
				[](const NonDefaultConstructible& wrap) {
					CHECK(wrap.m_value == 42);
					// Note: copy constructor creates a copy which is moved into the storage
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("converting copy constructing from a value")
	{
		const char forty_two = '*';
		Maybe<int> dut{forty_two};
		Maybe<DefaultConstructible> dut_default(dut);
		const Maybe<NonDefaultConstructible> dut_no_default(dut);

		THEN("the Maybes contains the value")
		{
			dut.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 42);
					// Note: copy constructor creates a copy which is moved into the storage
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_default.match(
				[](const NonDefaultConstructible& wrap) {
					CHECK(wrap.m_value == 42);
					// Note: copy constructor creates a copy which is moved into the storage
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("converting move constructing from an empty Maybe")
	{
		Maybe<int> dut_default_empty{std::nullopt};
		Maybe<DefaultConstructible> dut_default{std::move(dut_default_empty)};
		Maybe<int> dut_no_default_empty{std::nullopt};
		const Maybe<NonDefaultConstructible> dut_no_default{std::move(dut_no_default_empty)};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(dut_default_empty.has_value());
			CHECK_FALSE(dut_default.has_value());
			CHECK_FALSE(dut_no_default_empty.has_value());
			CHECK_FALSE(dut_no_default.has_value());
		}
	}

	WHEN("converting move constructing an non-empty Maybe")
	{
		Maybe<int> dut_default_value{33};
		Maybe<DefaultConstructible> dut_default1{std::move(dut_default_value)};
		Maybe<int> dut_no_default_empty{42};
		Maybe<NonDefaultConstructible> dut_no_default1{std::move(dut_no_default_empty)};

		const int default_value = 34;
		Maybe<DefaultConstructible> dut_default2{std::move(default_value)};
		const int no_default_value = 43;
		Maybe<NonDefaultConstructible> dut_no_default2{std::move(no_default_value)};

		THEN("the Maybes contain the value, the sources are empty")
		{
			CHECK(dut_default_value.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			dut_default1.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 33);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			CHECK(dut_no_default_empty.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
			dut_no_default1.match(
				[](const NonDefaultConstructible& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_default2.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 34);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_default2.match(
				[](const NonDefaultConstructible& wrap) {
					CHECK(wrap.m_value == 43);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("converting move constructing from a value")
	{
		DefaultConstructible default_value{66};
		Maybe<DefaultConstructible> dut_default(std::move(default_value));
		NonDefaultConstructible no_default_value{77};
		const Maybe<NonDefaultConstructible> dut_no_default(std::move(no_default_value));

		THEN("the Maybes contain the value, the source is empty")
		{
			CHECK(default_value.m_method == Method::MovedFrom);
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 66);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });

			CHECK(no_default_value.m_method == Method::MovedFrom);
			dut_no_default.match(
				[](const NonDefaultConstructible& wrap) {
					CHECK(wrap.m_value == 77);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("in-place construction")
	{
		using pair_t = std::pair<char, int>;
		Maybe<pair_t> dut(std::in_place, '4', 2);

		const char value = '4';
		const int number = 2;
		Maybe<const pair_t> dut_const(std::in_place, value, number);

		THEN("the Maybe contains the value")
		{
			dut.match(
				[](const pair_t& pair) {
					CHECK(pair.first == '4');
					CHECK(pair.second == 2);
				},
				[](std::nullopt_t) { CHECK(false); });

			dut_const.match(
				[](const pair_t& pair) {
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
				[](const vec_t& vec) {
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

SCENARIO("Maybe: assignments")
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
		Maybe<DefaultConstructible> dut_empty_lhs;
		Maybe<DefaultConstructible> dut_empty_rhs;
		Maybe<DefaultConstructible> dut_value_lhs(42);
		Maybe<DefaultConstructible> dut_value_rhs(33);

		WHEN("lhs has a value, rhs has a value")
		{
			dut_value_lhs = dut_value_rhs;

			THEN("the Maybes hold the same value")
			{
				dut_value_lhs.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::CopyAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });
				dut_value_rhs.match(
					[](const DefaultConstructible& wrap) {
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
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 33);
						// Note: copy constructor creates a copy which is moved into the storage
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				dut_value_rhs.match(
					[](const DefaultConstructible& wrap) {
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
		Maybe<DefaultConstructible> dut_empty_lhs;
		Maybe<DefaultConstructible> dut_empty_rhs;
		Maybe<DefaultConstructible> dut_value_lhs(42);
		Maybe<DefaultConstructible> dut_value_rhs(33);

		WHEN("lhs has a value, rhs has a value")
		{
			dut_value_lhs = std::move(dut_value_rhs);

			THEN("the Maybes hold the same value")
			{
				dut_value_lhs.match(
					[](const DefaultConstructible& wrap) {
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
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::MoveConstructed);
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
		DefaultConstructible value{101};
		Maybe<DefaultConstructible> dut = std::move(value);

		THEN("the Maybes contains the value")
		{
			dut.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 101);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}
}

SCENARIO("Maybe: modifiers")
{
	GIVEN("an empty Maybe")
	{
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move;
		Maybe<DefaultConstructible> dut_empty;

		WHEN("calling reset")
		{
			dut_no_copy_no_move.reset();

			THEN("the Maybe is empty")
			{
				CHECK_FALSE(dut_no_copy_no_move.has_value());
			}
		}

		WHEN("calling emplace (copy)")
		{
			const auto& result = dut_no_copy_no_move.emplace(11);

			THEN("the Maybe holds the value")
			{
				CHECK(dut_no_copy_no_move.has_value());
				CHECK(dut_no_copy_no_move.match(
					[](const NonCopyableNonMovable& value) {
						return value.m_value == 11 && value.m_method == Method::ValueConstructed;
					},
					[](std::nullopt_t) { return false; }));
				CHECK(result.m_value == 11);
			}
		}

		WHEN("calling emplace (move)")
		{
			auto value = DefaultConstructible{42};
			const auto& result = dut_empty.emplace(std::move(value));

			THEN("the Maybe holds the value")
			{
				CHECK(dut_empty.has_value());
				CHECK(dut_empty.match(
					[](const DefaultConstructible& wrap) {
						return wrap.m_value == 42 && wrap.m_method == Method::MoveConstructed;
					},
					[](std::nullopt_t) { return false; }));
				CHECK(result.m_value == 42);
			}
		}

		WHEN("calling swap with an empty Maybe")
		{
			Maybe<DefaultConstructible> dut_empty2;
			dut_empty.swap(dut_empty2);

			THEN("the values are swapped")
			{
				CHECK(dut_empty.match([](const DefaultConstructible&) { return false; },
									  [](std::nullopt_t) { return true; }));
				CHECK(dut_empty2.match([](const DefaultConstructible&) { return false; },
									   [](std::nullopt_t) { return true; }));
			}
		}

		WHEN("calling swap with an non-empty Maybe")
		{
			auto dut_value = Maybe<DefaultConstructible>{42};
			dut_empty.swap(dut_value);

			THEN("the values are swapped")
			{
				CHECK(dut_empty.match(
					[](const DefaultConstructible& wrap) {
						return wrap.m_value == 42 && wrap.m_method == Method::MoveConstructed;
					},
					[](std::nullopt_t) { return false; }));
				CHECK(dut_value.match([](const DefaultConstructible&) { return false; },
									  [](std::nullopt_t) { return true; }));
			}
		}
	}

	GIVEN("an Maybe with a value")
	{
		Maybe<DefaultConstructible> dut_value = DefaultConstructible{4};
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move{42};

		WHEN("calling reset")
		{
			dut_no_copy_no_move.reset();

			THEN("the Maybe is empty")
			{
				CHECK_FALSE(dut_no_copy_no_move.has_value());
			}
		}

		WHEN("calling emplace (copy)")
		{
			const auto& result = dut_no_copy_no_move.emplace(11);

			THEN("the Maybe holds the value")
			{
				CHECK(dut_no_copy_no_move.has_value());
				CHECK(dut_no_copy_no_move.match(
					[](const NonCopyableNonMovable& value) {
						return value.m_value == 11 && value.m_method == Method::ValueConstructed;
					},
					[](std::nullopt_t) { return false; }));
				CHECK(result.m_value == 11);
			}
		}

		WHEN("calling emplace (move)")
		{
			auto value = DefaultConstructible{42};
			const auto& result = dut_value.emplace(std::move(value));

			THEN("the Maybe holds the value")
			{
				CHECK(dut_value.has_value());
				CHECK(dut_value.match(
					[](const DefaultConstructible& wrap) {
						return wrap.m_value == 42 && wrap.m_method == Method::MoveConstructed;
					},
					[](std::nullopt_t) { return false; }));
				CHECK(result.m_value == 42);
			}
		}

		WHEN("calling swap with an empty Maybe")
		{
			Maybe<DefaultConstructible> dut_empty;
			dut_value.swap(dut_empty);

			THEN("the values are swapped")
			{
				CHECK(dut_value.match([](const DefaultConstructible&) { return false; },
									  [](std::nullopt_t) { return true; }));
				CHECK(dut_empty.match(
					[](const DefaultConstructible& wrap) {
						return wrap.m_value == 4 && wrap.m_method == Method::MoveConstructed;
					},
					[](std::nullopt_t) { return false; }));
			}
		}

		WHEN("calling swap with a non-empty Maybe")
		{
			Maybe<DefaultConstructible> dut_value2 = DefaultConstructible{42};
			dut_value.swap(dut_value2);

			THEN("the values are swapped")
			{
				CHECK(dut_value.match(
					[](const DefaultConstructible& wrap) {
						return wrap.m_value == 42 && wrap.m_method == Method::MoveAssigned;
					},
					[](std::nullopt_t) { return false; }));
				CHECK(dut_value2.match(
					[](const DefaultConstructible& wrap) {
						return wrap.m_value == 4 && wrap.m_method == Method::MoveAssigned;
					},
					[](std::nullopt_t) { return false; }));
			}
		}
	}
}

SCENARIO("Maybe: observers")
{
	GIVEN("an empty Maybe")
	{
		Maybe<int> dut;

		WHEN("converting to boolean")
		{
			THEN("returns false")
			{
				CHECK_FALSE(dut);
			}
		}

		WHEN("calling has_value")
		{
			THEN("returns false")
			{
				CHECK_FALSE(dut.has_value());
			}
		}

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

		WHEN("converting to boolean")
		{
			THEN("returns true")
			{
				CHECK(dut);
			}
		}

		WHEN("calling has_value")
		{
			THEN("returns true")
			{
				CHECK(dut.has_value());
			}
		}

		WHEN("calling match")
		{
			THEN("the value can be retrieved")
			{
				CHECK(dut.match([](int value) { return value == 44; }, [](std::nullopt_t) { return false; }));
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

SCENARIO("Maybe: monadic operations")
{
	GIVEN("an Maybe with a value")
	{
		Maybe<int> dut = 11;

		WHEN("calling and_then")
		{
			auto int_to_maybe_string = [](int value) { return Maybe<std::string>(std::to_string(value)); };
			auto result = dut.and_then(int_to_maybe_string);
			THEN("the result is a Maybe with the transformed value")
			{
				CHECK(result.has_value());
				CHECK(result.match([](const std::string& str) { return str == "11"; },
								   [](std::nullopt_t) { return false; }));
				CHECK(dut.has_value());
				CHECK(dut.match([](int value) { return value == 11; }, [](std::nullopt_t) { return false; }));
			}
		}

		WHEN("calling transform")
		{
			auto int_to_string = [](int value) { return std::to_string(value); };
			auto result = dut.transform(int_to_string);
			THEN("the result is a Maybe with the transformed value")
			{
				CHECK(result.has_value());
				CHECK(result.match([](const std::string& str) { return str == "11"; },
								   [](std::nullopt_t) { return false; }));
				CHECK(dut.has_value());
				CHECK(dut.match([](int value) { return value == 11; }, [](std::nullopt_t) { return false; }));
			}
		}

		WHEN("calling or_else")
		{
			auto fallback = []() { return Maybe<int>(0); };
			auto result = dut.or_else(fallback);
			THEN("the result is the original Maybe")
			{
				CHECK(result.has_value());
				CHECK(result.match([](int value) { return value == 11; }, [](std::nullopt_t) { return false; }));
				CHECK(dut.has_value());
				CHECK(dut.match([](int value) { return value == 11; }, [](std::nullopt_t) { return false; }));
			}

			dut.reset();
			result = dut.or_else(fallback);
			THEN("the result is the fallback Maybe")
			{
				CHECK(result.has_value());
				CHECK(result.match([](int value) { return value == 0; }, [](std::nullopt_t) { return false; }));
				CHECK_FALSE(dut.has_value());
			}
		}
	}
}

// SCENARIO("Maybe: invalid types")
// {
// 	// Maybe<std::in_place_t> dut1{};
// 	// Maybe<void> dut2{};
// 	// Maybe<std::nullopt_t> dut3{};
// }
