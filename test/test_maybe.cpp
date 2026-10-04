/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/types/maybe.hpp"

#include <initializer_list>
#include <string>
#include <type_traits>

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
	ValueAssigned,
	MoveAssigned,
};

class DefaultConstructible
{
public:
	DefaultConstructible(int value = 42)
		: m_value{value}
		, m_method{Method::DefaultConstructed}
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

class ExplicitConstructible
{
public:
	explicit ExplicitConstructible(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
		, m_string{"ValueConstructed"}
	{
	}

	ExplicitConstructible(ExplicitConstructible const& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::CopyConstructed)
		, m_string(rhs.m_string)
	{
	}

	ExplicitConstructible& operator=(ExplicitConstructible const& rhs)
	{
		m_value = rhs.m_value;
		m_method = Method::CopyAssigned;
		m_string = rhs.m_string;
		return *this;
	}

	ExplicitConstructible(ExplicitConstructible&& rhs) noexcept
		: m_value(rhs.m_value)
		, m_method(Method::MoveConstructed)
		, m_string(std::move(rhs.m_string))
	{
		rhs.m_method = Method::MovedFrom;
	}

	ExplicitConstructible& operator=(ExplicitConstructible&& rhs) noexcept
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

static_assert(!std::is_trivially_constructible_v<ExplicitConstructible>);
static_assert(!std::is_trivially_copyable_v<ExplicitConstructible>);
static_assert(!std::is_trivially_move_constructible_v<ExplicitConstructible>);
static_assert(!std::is_default_constructible_v<ExplicitConstructible>);
static_assert(std::is_copy_constructible_v<ExplicitConstructible>);
static_assert(std::is_copy_assignable_v<ExplicitConstructible>);
static_assert(std::is_move_constructible_v<ExplicitConstructible>);
static_assert(std::is_move_assignable_v<ExplicitConstructible>);

class CopyableOnly
{
public:
	CopyableOnly()
		: m_value{42}
		, m_method{Method::DefaultConstructed}
		, m_string{"DefaultConstructed"}
	{
	}

	explicit CopyableOnly(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
		, m_string{"ValueConstructed"}
	{
	}

	CopyableOnly(CopyableOnly const& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::CopyConstructed)
		, m_string(rhs.m_string)
	{
	}

	CopyableOnly& operator=(CopyableOnly const& rhs)
	{
		m_value = rhs.m_value;
		m_method = Method::CopyAssigned;
		m_string = rhs.m_string;
		return *this;
	}

	CopyableOnly(CopyableOnly&& rhs) = delete;
	CopyableOnly& operator=(CopyableOnly&& rhs) = delete;

	int m_value;
	Method m_method;
	std::string m_string;
};

static_assert(!std::is_trivially_constructible_v<CopyableOnly>);
static_assert(!std::is_trivially_copyable_v<CopyableOnly>);
static_assert(!std::is_trivially_move_constructible_v<CopyableOnly>);
static_assert(std::is_default_constructible_v<CopyableOnly>);
static_assert(std::is_copy_constructible_v<CopyableOnly>);
static_assert(std::is_copy_assignable_v<CopyableOnly>);
static_assert(!std::is_move_constructible_v<CopyableOnly>);
static_assert(!std::is_move_assignable_v<CopyableOnly>);

class MoveableOnly
{
public:
	MoveableOnly()
		: m_value{42}
		, m_method{Method::DefaultConstructed}
		, m_string{"DefaultConstructed"}
	{
	}

	explicit MoveableOnly(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
		, m_string{"ValueConstructed"}
	{
	}

	MoveableOnly(MoveableOnly const& rhs) = delete;
	MoveableOnly& operator=(MoveableOnly const& rhs) = delete;

	MoveableOnly(MoveableOnly&& rhs) noexcept
		: m_value(rhs.m_value)
		, m_method(Method::MoveConstructed)
		, m_string(std::move(rhs.m_string))
	{
		rhs.m_method = Method::MovedFrom;
	}

	MoveableOnly& operator=(MoveableOnly&& rhs) noexcept
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

static_assert(!std::is_trivially_constructible_v<MoveableOnly>);
static_assert(!std::is_trivially_copyable_v<MoveableOnly>);
static_assert(!std::is_trivially_move_constructible_v<MoveableOnly>);
static_assert(std::is_default_constructible_v<MoveableOnly>);
static_assert(!std::is_copy_constructible_v<MoveableOnly>);
static_assert(!std::is_copy_assignable_v<MoveableOnly>);
static_assert(std::is_move_constructible_v<MoveableOnly>);
static_assert(std::is_move_assignable_v<MoveableOnly>);

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

class CopyConstructibleOnly
{
public:
	CopyConstructibleOnly()
		: m_value{42}
		, m_method{Method::DefaultConstructed}
		, m_string{"DefaultConstructed"}
	{
	}

	explicit CopyConstructibleOnly(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
		, m_string{"ValueConstructed"}
	{
	}

	CopyConstructibleOnly(CopyConstructibleOnly const& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::CopyConstructed)
		, m_string(rhs.m_string)
	{
	}

	CopyConstructibleOnly& operator=(CopyConstructibleOnly const& rhs) = delete;
	CopyConstructibleOnly(CopyConstructibleOnly&& rhs) = delete;
	CopyConstructibleOnly& operator=(CopyConstructibleOnly&& rhs) = delete;

	int m_value;
	Method m_method;
	std::string m_string;
};

static_assert(!std::is_trivially_constructible_v<CopyConstructibleOnly>);
static_assert(!std::is_trivially_copyable_v<CopyConstructibleOnly>);
static_assert(!std::is_trivially_move_constructible_v<CopyConstructibleOnly>);
static_assert(std::is_default_constructible_v<CopyConstructibleOnly>);
static_assert(std::is_copy_constructible_v<CopyConstructibleOnly>);
static_assert(!std::is_copy_assignable_v<CopyConstructibleOnly>);
static_assert(!std::is_move_constructible_v<CopyConstructibleOnly>);
static_assert(!std::is_move_assignable_v<CopyConstructibleOnly>);

class MoveConstructibleOnly
{
public:
	MoveConstructibleOnly()
		: m_value{42}
		, m_method{Method::DefaultConstructed}
		, m_string{"DefaultConstructed"}
	{
	}

	explicit MoveConstructibleOnly(int value)
		: m_value{value}
		, m_method{Method::ValueConstructed}
		, m_string{"ValueConstructed"}
	{
	}

	MoveConstructibleOnly(MoveConstructibleOnly const& rhs) = delete;
	MoveConstructibleOnly& operator=(MoveConstructibleOnly const& rhs) = delete;

	MoveConstructibleOnly(MoveConstructibleOnly&& rhs)
		: m_value(rhs.m_value)
		, m_method(Method::MoveConstructed)
		, m_string(std::move(rhs.m_string))
	{
		rhs.m_method = Method::MovedFrom;
	}

	MoveConstructibleOnly& operator=(MoveConstructibleOnly&& rhs) = delete;

	int m_value;
	Method m_method;
	std::string m_string;
};

static_assert(!std::is_trivially_constructible_v<MoveConstructibleOnly>);
static_assert(!std::is_trivially_copyable_v<MoveConstructibleOnly>);
static_assert(!std::is_trivially_move_constructible_v<MoveConstructibleOnly>);
static_assert(std::is_default_constructible_v<MoveConstructibleOnly>);
static_assert(!std::is_copy_constructible_v<MoveConstructibleOnly>);
static_assert(!std::is_copy_assignable_v<MoveConstructibleOnly>);
static_assert(std::is_move_constructible_v<MoveConstructibleOnly>);
static_assert(!std::is_move_assignable_v<MoveConstructibleOnly>);

class NonMovableList
{
public:
	explicit NonMovableList(std::initializer_list<int> values)
		: m_size{values.size()}
	{
	}

	NonMovableList(const NonMovableList&) = delete;
	NonMovableList(NonMovableList&&) = delete;

	std::size_t m_size;
};

struct ReturnsInt
{
	int operator()(int value) const
	{
		return value;
	}
};

struct ReturnsMaybeInt
{
	Maybe<int> operator()(int value) const
	{
		return Maybe<int>{value};
	}
};

struct ReturnsVoid
{
	void operator()(int) const {}
};

struct ReturnsIntFallback
{
	int operator()() const
	{
		return 0;
	}
};

struct ReturnsMaybeIntFallback
{
	Maybe<int> operator()() const
	{
		return Maybe<int>{0};
	}
};

template <class F>
concept MaybeIntAndThenCallable = requires(Maybe<int>& value, F callback) { value.and_then(callback); };

template <class F>
concept MaybeIntTransformCallable = requires(Maybe<int>& value, F callback) { value.transform(callback); };

template <class F>
concept MaybeIntOrElseCallable = requires(Maybe<int>& value, F callback) { value.or_else(callback); };

static_assert(!MaybeIntAndThenCallable<ReturnsInt>);
static_assert(MaybeIntAndThenCallable<ReturnsMaybeInt>);
static_assert(MaybeIntTransformCallable<ReturnsInt>);
static_assert(!MaybeIntTransformCallable<ReturnsVoid>);
static_assert(!MaybeIntOrElseCallable<ReturnsIntFallback>);
static_assert(MaybeIntOrElseCallable<ReturnsMaybeIntFallback>);

struct MatchValue
{
	bool operator()(int&) const
	{
		return true;
	}
};

struct MatchConstValue
{
	bool operator()(const int&) const
	{
		return true;
	}
};

struct MatchEmpty
{
	bool operator()(std::nullopt_t) const
	{
		return false;
	}
};

struct MatchGenericOne
{
	template <class U>
	bool operator()(U&&) const
	{
		return true;
	}
};

struct MatchGenericTwo
{
	template <class U>
	bool operator()(U&&) const
	{
		return false;
	}
};

template <class M, class... F>
concept MaybeMatchCallable = requires(M& value, F... handlers) { value.match(handlers...); };

static_assert(MaybeMatchCallable<Maybe<int>, MatchValue, MatchEmpty>);
static_assert(MaybeMatchCallable<Maybe<int>, MatchEmpty, MatchValue>);
static_assert(!MaybeMatchCallable<Maybe<int>, MatchValue>);
static_assert(!MaybeMatchCallable<Maybe<int>, MatchValue, MatchEmpty, MatchEmpty>);
static_assert(!MaybeMatchCallable<Maybe<int>, MatchValue, MatchValue>);
static_assert(!MaybeMatchCallable<Maybe<int>, MatchGenericOne, MatchGenericTwo>);
static_assert(MaybeMatchCallable<const Maybe<int>, MatchConstValue, MatchEmpty>);
static_assert(!MaybeMatchCallable<const Maybe<int>, MatchValue, MatchEmpty>);
}  // namespace

SCENARIO("Maybe: type traits")
{
	static_assert(std::is_constructible_v<Maybe<int>, std::nullopt_t>);
	static_assert(std::is_constructible_v<Maybe<int>, std::in_place_t, int>);
	static_assert(!std::is_constructible_v<Maybe<int>, std::in_place_t, std::string>);

	static_assert(std::is_default_constructible_v<Maybe<int>>);
	static_assert(std::is_copy_constructible_v<Maybe<int>>);
	static_assert(std::is_copy_assignable_v<Maybe<int>>);
	static_assert(std::is_move_constructible_v<Maybe<int>>);
	static_assert(std::is_move_assignable_v<Maybe<int>>);

	static_assert(std::is_default_constructible_v<DefaultConstructible>);
	static_assert(std::is_copy_constructible_v<DefaultConstructible>);
	static_assert(std::is_copy_assignable_v<DefaultConstructible>);
	static_assert(std::is_move_constructible_v<DefaultConstructible>);
	static_assert(std::is_move_assignable_v<DefaultConstructible>);

	static_assert(std::is_default_constructible_v<Maybe<DefaultConstructible>>);
	static_assert(std::is_copy_constructible_v<Maybe<DefaultConstructible>>);
	static_assert(std::is_copy_assignable_v<Maybe<DefaultConstructible>>);
	static_assert(std::is_move_constructible_v<Maybe<DefaultConstructible>>);
	static_assert(std::is_move_assignable_v<Maybe<DefaultConstructible>>);

	static_assert(!std::is_default_constructible_v<ExplicitConstructible>);
	static_assert(std::is_copy_constructible_v<ExplicitConstructible>);
	static_assert(std::is_copy_assignable_v<ExplicitConstructible>);
	static_assert(std::is_move_constructible_v<ExplicitConstructible>);
	static_assert(std::is_move_assignable_v<ExplicitConstructible>);

	static_assert(std::is_default_constructible_v<Maybe<ExplicitConstructible>>);
	static_assert(std::is_copy_constructible_v<Maybe<ExplicitConstructible>>);
	static_assert(std::is_copy_assignable_v<Maybe<ExplicitConstructible>>);
	static_assert(std::is_move_constructible_v<Maybe<ExplicitConstructible>>);
	static_assert(std::is_move_assignable_v<Maybe<ExplicitConstructible>>);

	static_assert(std::is_default_constructible_v<CopyableOnly>);
	static_assert(std::is_copy_constructible_v<CopyableOnly>);
	static_assert(std::is_copy_assignable_v<CopyableOnly>);
	static_assert(!std::is_move_constructible_v<CopyableOnly>);
	static_assert(!std::is_move_assignable_v<CopyableOnly>);

	static_assert(std::is_default_constructible_v<Maybe<CopyableOnly>>);
	static_assert(std::is_copy_constructible_v<Maybe<CopyableOnly>>);
	static_assert(std::is_copy_assignable_v<Maybe<CopyableOnly>>);
	static_assert(!std::is_move_constructible_v<Maybe<CopyableOnly>>);
	static_assert(!std::is_move_assignable_v<Maybe<CopyableOnly>>);

	static_assert(std::is_default_constructible_v<MoveableOnly>);
	static_assert(!std::is_copy_constructible_v<MoveableOnly>);
	static_assert(!std::is_copy_assignable_v<MoveableOnly>);
	static_assert(std::is_move_constructible_v<MoveableOnly>);
	static_assert(std::is_move_assignable_v<MoveableOnly>);

	static_assert(std::is_default_constructible_v<Maybe<MoveableOnly>>);
	static_assert(!std::is_copy_constructible_v<Maybe<MoveableOnly>>);
	static_assert(!std::is_copy_assignable_v<Maybe<MoveableOnly>>);
	static_assert(std::is_move_constructible_v<Maybe<MoveableOnly>>);
	static_assert(std::is_move_assignable_v<Maybe<MoveableOnly>>);

	static_assert(std::is_default_constructible_v<NonCopyableNonMovable>);
	static_assert(!std::is_copy_constructible_v<NonCopyableNonMovable>);
	static_assert(!std::is_copy_assignable_v<NonCopyableNonMovable>);
	static_assert(!std::is_move_constructible_v<NonCopyableNonMovable>);
	static_assert(!std::is_move_assignable_v<NonCopyableNonMovable>);

	static_assert(std::is_default_constructible_v<Maybe<NonCopyableNonMovable>>);
	static_assert(!std::is_copy_constructible_v<Maybe<NonCopyableNonMovable>>);
	static_assert(!std::is_copy_assignable_v<Maybe<NonCopyableNonMovable>>);
	static_assert(!std::is_move_constructible_v<Maybe<NonCopyableNonMovable>>);
	static_assert(!std::is_move_assignable_v<Maybe<NonCopyableNonMovable>>);

	static_assert(std::is_default_constructible_v<CopyConstructibleOnly>);
	static_assert(std::is_copy_constructible_v<CopyConstructibleOnly>);
	static_assert(!std::is_copy_assignable_v<CopyConstructibleOnly>);
	static_assert(!std::is_move_constructible_v<CopyConstructibleOnly>);
	static_assert(!std::is_move_assignable_v<CopyConstructibleOnly>);

	static_assert(std::is_default_constructible_v<Maybe<CopyConstructibleOnly>>);
	static_assert(std::is_copy_constructible_v<Maybe<CopyConstructibleOnly>>);
	static_assert(!std::is_copy_assignable_v<Maybe<CopyConstructibleOnly>>);
	static_assert(!std::is_move_constructible_v<Maybe<CopyConstructibleOnly>>);
	static_assert(!std::is_move_assignable_v<Maybe<CopyConstructibleOnly>>);

	static_assert(std::is_default_constructible_v<MoveConstructibleOnly>);
	static_assert(!std::is_copy_constructible_v<MoveConstructibleOnly>);
	static_assert(!std::is_copy_assignable_v<MoveConstructibleOnly>);
	static_assert(std::is_move_constructible_v<MoveConstructibleOnly>);
	static_assert(!std::is_move_assignable_v<MoveConstructibleOnly>);

	static_assert(std::is_default_constructible_v<Maybe<MoveConstructibleOnly>>);
	static_assert(!std::is_copy_constructible_v<Maybe<MoveConstructibleOnly>>);
	static_assert(!std::is_copy_assignable_v<Maybe<MoveConstructibleOnly>>);
	static_assert(std::is_move_constructible_v<Maybe<MoveConstructibleOnly>>);
	static_assert(!std::is_move_assignable_v<Maybe<MoveConstructibleOnly>>);
}

SCENARIO("Maybe: constructors")
{
	WHEN("using the default constructor")
	{
		Maybe<int> dut_primitive{};
		Maybe<DefaultConstructible> dut_default{};
		Maybe<ExplicitConstructible> dut_explicit{};
		Maybe<CopyableOnly> dut_copy{};
		Maybe<MoveableOnly> dut_move{};
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move{};
		Maybe<CopyConstructibleOnly> dut_no_copy_assign{};
		Maybe<MoveConstructibleOnly> dut_no_move_assign{};

		THEN("all Maybes are empty")
		{
			CHECK_FALSE(dut_primitive.has_value());
			CHECK_FALSE(dut_default.has_value());
			CHECK_FALSE(dut_explicit.has_value());
			CHECK_FALSE(dut_copy.has_value());
			CHECK_FALSE(dut_move.has_value());
			CHECK_FALSE(dut_no_copy_no_move.has_value());
			CHECK_FALSE(dut_no_copy_assign.has_value());
			CHECK_FALSE(dut_no_move_assign.has_value());
		}
	}

	WHEN("explicitly constructing an empty Maybe")
	{
		Maybe<int> dut_primitive{std::nullopt};
		Maybe<DefaultConstructible> dut_default{std::nullopt};
		Maybe<ExplicitConstructible> dut_explicit{std::nullopt};
		Maybe<CopyableOnly> dut_copy{std::nullopt};
		Maybe<MoveableOnly> dut_move{std::nullopt};
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move{std::nullopt};
		Maybe<CopyConstructibleOnly> dut_no_copy_assign{std::nullopt};
		Maybe<MoveConstructibleOnly> dut_no_move_assign{std::nullopt};

		THEN("all Maybes are empty")
		{
			CHECK_FALSE(dut_primitive.has_value());
			CHECK_FALSE(dut_default.has_value());
			CHECK_FALSE(dut_explicit.has_value());
			CHECK_FALSE(dut_copy.has_value());
			CHECK_FALSE(dut_move.has_value());
			CHECK_FALSE(dut_no_copy_no_move.has_value());
			CHECK_FALSE(dut_no_copy_assign.has_value());
			CHECK_FALSE(dut_no_move_assign.has_value());
		}
	}

	WHEN("construction from an value")
	{
		static_assert(std::is_constructible_v<Maybe<int>, int>);
		static_assert(!std::is_convertible_v<int, Maybe<int>>);
		static_assert(std::is_constructible_v<Maybe<int>, int&>);
		static_assert(std::is_constructible_v<Maybe<int>, const int&>);
		static_assert(std::is_constructible_v<Maybe<int>, int&&>);
		static_assert(std::is_constructible_v<Maybe<int>, const int&&>);
		static_assert(!std::is_convertible_v<int&, Maybe<int>>);
		static_assert(!std::is_convertible_v<const int&, Maybe<int>>);
		static_assert(!std::is_convertible_v<int&&, Maybe<int>>);
		static_assert(!std::is_convertible_v<const int&&, Maybe<int>>);
		static_assert(std::is_constructible_v<Maybe<DefaultConstructible>, int>);
		static_assert(!std::is_convertible_v<int, Maybe<DefaultConstructible>>);
		static_assert(std::is_constructible_v<Maybe<ExplicitConstructible>, int>);
		static_assert(!std::is_convertible_v<int, Maybe<ExplicitConstructible>>);
		static_assert(std::is_constructible_v<Maybe<CopyableOnly>, int>);
		static_assert(!std::is_convertible_v<int, Maybe<CopyableOnly>>);
		static_assert(std::is_constructible_v<Maybe<MoveableOnly>, int>);
		static_assert(!std::is_convertible_v<int, Maybe<MoveableOnly>>);
		static_assert(std::is_constructible_v<Maybe<NonCopyableNonMovable>, int>);
		static_assert(!std::is_convertible_v<int, Maybe<NonCopyableNonMovable>>);
		static_assert(std::is_constructible_v<Maybe<CopyConstructibleOnly>, int>);
		static_assert(!std::is_convertible_v<int, Maybe<CopyConstructibleOnly>>);
		static_assert(std::is_constructible_v<Maybe<MoveConstructibleOnly>, int>);
		static_assert(!std::is_convertible_v<int, Maybe<MoveConstructibleOnly>>);

		Maybe<int> dut_primitive(102);
		Maybe<DefaultConstructible> dut_default(100);
		const Maybe<ExplicitConstructible> dut_explicit(200);
		Maybe<CopyableOnly> dut_copy(300);
		Maybe<MoveableOnly> dut_move(400);
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move(500);
		Maybe<CopyConstructibleOnly> dut_no_copy_assign(600);
		Maybe<MoveConstructibleOnly> dut_no_move_assign(700);

		THEN("the Maybes contains the value")
		{
			dut_primitive.match([](int val) { CHECK(val == 102); }, [](std::nullopt_t) { CHECK(false); });
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 100);
					CHECK(wrap.m_method == Method::DefaultConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit.match(
				[](const ExplicitConstructible& wrap) {
					CHECK(wrap.m_value == 200);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_copy.match(
				[](const CopyableOnly& wrap) {
					CHECK(wrap.m_value == 300);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_move.match(
				[](const MoveableOnly& wrap) {
					CHECK(wrap.m_value == 400);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_copy_no_move.match(
				[](const NonCopyableNonMovable& wrap) {
					CHECK(wrap.m_value == 500);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_copy_assign.match(
				[](const CopyConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 600);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_move_assign.match(
				[](const MoveConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 700);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("copy constructing from an empty Maybe")
	{
		Maybe<int> dut_primitive_empty{std::nullopt};
		Maybe<int> dut_primitive{dut_primitive_empty};
		Maybe<DefaultConstructible> dut_default_empty{std::nullopt};
		Maybe<DefaultConstructible> dut_default{dut_default_empty};
		const Maybe<ExplicitConstructible> dut_explicit_empty{std::nullopt};
		const Maybe<ExplicitConstructible> dut_explicit{dut_explicit_empty};
		Maybe<CopyableOnly> dut_copy_empty{std::nullopt};
		Maybe<CopyableOnly> dut_copy{dut_copy_empty};
		static_assert(!std::is_constructible_v<Maybe<MoveableOnly>, const Maybe<MoveableOnly>&>);
		static_assert(!std::is_constructible_v<Maybe<MoveConstructibleOnly>, const Maybe<MoveConstructibleOnly>&>);
		Maybe<CopyConstructibleOnly> dut_no_copy_assign_empty{std::nullopt};
		Maybe<CopyConstructibleOnly> dut_no_copy_assign{dut_no_copy_assign_empty};
		static_assert(!std::is_constructible_v<Maybe<NonCopyableNonMovable>, const Maybe<NonCopyableNonMovable>&>);

		THEN("all Maybes are empty")
		{
			CHECK_FALSE(dut_primitive.has_value());
			CHECK_FALSE(dut_default.has_value());
			CHECK_FALSE(dut_explicit.has_value());
			CHECK_FALSE(dut_copy.has_value());
			CHECK_FALSE(dut_no_copy_assign.has_value());
		}
	}

	WHEN("copy constructing from a non-empty Maybe")
	{
		Maybe<int> dut_primitive_value{42};
		Maybe<int> dut_primitive{dut_primitive_value};
		Maybe<DefaultConstructible> dut_default_value{53};
		Maybe<DefaultConstructible> dut_default{dut_default_value};
		const Maybe<ExplicitConstructible> dut_explicit_value{ExplicitConstructible{64}};
		const Maybe<ExplicitConstructible> dut_explicit{dut_explicit_value};
		const CopyableOnly copyable_only_value{75};
		Maybe<CopyableOnly> dut_copy_value{copyable_only_value};
		Maybe<CopyableOnly> dut_copy{dut_copy_value};
		static_assert(!std::is_constructible_v<Maybe<MoveableOnly>, const Maybe<MoveableOnly>&>);
		static_assert(!std::is_constructible_v<Maybe<NonCopyableNonMovable>, const Maybe<NonCopyableNonMovable>&>);
		const CopyConstructibleOnly copy_constructible_only_value{108};
		Maybe<CopyConstructibleOnly> dut_no_copy_assign_value{copy_constructible_only_value};
		Maybe<CopyConstructibleOnly> dut_no_copy_assign{dut_no_copy_assign_value};
		static_assert(!std::is_constructible_v<Maybe<MoveConstructibleOnly>, const Maybe<MoveConstructibleOnly>&>);

		THEN("the Maybes contains the value")
		{
			dut_primitive.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 53);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit.match(
				[](const ExplicitConstructible& wrap) {
					CHECK(wrap.m_value == 64);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_copy.match(
				[](const CopyableOnly& wrap) {
					CHECK(wrap.m_value == 75);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_copy_assign.match(
				[](const CopyConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 108);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("copy constructing from a value")
	{
		const int primitive_value = 33;
		Maybe<int> dut_primitive{primitive_value};
		DefaultConstructible default_value{66};
		Maybe<DefaultConstructible> dut_default(default_value);
		const ExplicitConstructible explicit_value{44};
		const Maybe<ExplicitConstructible> dut_explicit(explicit_value);
		const CopyableOnly copyable_only_value{55};
		Maybe<CopyableOnly> dut_copy(copyable_only_value);
		static_assert(!std::is_constructible_v<Maybe<MoveableOnly>, MoveableOnly&>);
		static_assert(!std::is_constructible_v<Maybe<NonCopyableNonMovable>, NonCopyableNonMovable&>);
		const CopyConstructibleOnly copy_constructible_only_value{99};
		Maybe<CopyConstructibleOnly> dut_no_copy_assign(copy_constructible_only_value);
		static_assert(!std::is_constructible_v<Maybe<MoveConstructibleOnly>, MoveConstructibleOnly&>);

		THEN("the Maybes contains the value")
		{
			dut_primitive.match([](int val) { CHECK(val == 33); }, [](std::nullopt_t) { CHECK(false); });
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 66);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit.match(
				[](const ExplicitConstructible& wrap) {
					CHECK(wrap.m_value == 44);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_copy.match(
				[](const CopyableOnly& wrap) {
					CHECK(wrap.m_value == 55);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_copy_assign.match(
				[](const CopyConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 99);
					CHECK(wrap.m_method == Method::CopyConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("move constructing from an empty Maybe")
	{
		Maybe<int> dut_primitive_empty{std::nullopt};
		Maybe<int> dut_primitive{std::move(dut_primitive_empty)};
		const Maybe<DefaultConstructible> dut_default_empty{std::nullopt};
		const Maybe<DefaultConstructible> dut_default{std::move(dut_default_empty)};
		Maybe<ExplicitConstructible> dut_explicit_empty{std::nullopt};
		Maybe<ExplicitConstructible> dut_explicit{std::move(dut_explicit_empty)};
		static_assert(!std::is_constructible_v<Maybe<CopyableOnly>, Maybe<CopyableOnly>&&>);
		Maybe<MoveableOnly> dut_move_empty{std::nullopt};
		Maybe<MoveableOnly> dut_move{std::move(dut_move_empty)};
		static_assert(!std::is_constructible_v<Maybe<NonCopyableNonMovable>, Maybe<NonCopyableNonMovable>&&>);
		static_assert(!std::is_constructible_v<Maybe<CopyConstructibleOnly>, Maybe<CopyConstructibleOnly>&&>);
		Maybe<MoveConstructibleOnly> dut_no_move_assign_empty{std::nullopt};
		Maybe<MoveConstructibleOnly> dut_no_move_assign{std::move(dut_no_move_assign_empty)};

		THEN("all Maybes are empty")
		{
			CHECK_FALSE(dut_primitive_empty.has_value());
			CHECK_FALSE(dut_primitive.has_value());
			CHECK_FALSE(dut_default_empty.has_value());
			CHECK_FALSE(dut_default.has_value());
			CHECK_FALSE(dut_explicit_empty.has_value());
			CHECK_FALSE(dut_explicit.has_value());
			CHECK_FALSE(dut_move_empty.has_value());
			CHECK_FALSE(dut_move.has_value());
			CHECK_FALSE(dut_no_move_assign_empty.has_value());
			CHECK_FALSE(dut_no_move_assign.has_value());
		}
	}

	WHEN("move constructing from a non-empty Maybe")
	{
		Maybe<int> dut_primitive_value{42};
		Maybe<int> dut_primitive{std::move(dut_primitive_value)};
		Maybe<DefaultConstructible> dut_default_value{53};
		Maybe<DefaultConstructible> dut_default{std::move(dut_default_value)};
		Maybe<ExplicitConstructible> dut_explicit_value{64};
		const Maybe<ExplicitConstructible> dut_explicit{std::move(dut_explicit_value)};
		static_assert(!std::is_constructible_v<Maybe<CopyableOnly>, Maybe<CopyableOnly>&&>);
		Maybe<MoveableOnly> dut_move_value{MoveableOnly{75}};
		Maybe<MoveableOnly> dut_move{std::move(dut_move_value)};
		static_assert(!std::is_constructible_v<Maybe<NonCopyableNonMovable>, Maybe<NonCopyableNonMovable>&&>);
		static_assert(!std::is_constructible_v<Maybe<CopyConstructibleOnly>, Maybe<CopyConstructibleOnly>&&>);
		Maybe<MoveConstructibleOnly> dut_no_move_assign_value{MoveConstructibleOnly{99}};
		Maybe<MoveConstructibleOnly> dut_no_move_assign{std::move(dut_no_move_assign_value)};

		THEN("the Maybes contain the value, the sources are empty")
		{
			CHECK_FALSE(dut_primitive_value.has_value());
			CHECK_FALSE(dut_default_value.has_value());
			CHECK_FALSE(dut_explicit_value.has_value());
			CHECK_FALSE(dut_move_value.has_value());
			CHECK_FALSE(dut_no_move_assign_value.has_value());

			dut_primitive.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 53);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit.match(
				[](const ExplicitConstructible& wrap) {
					CHECK(wrap.m_value == 64);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_move.match(
				[](const MoveableOnly& wrap) {
					CHECK(wrap.m_value == 75);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_move_assign.match(
				[](const MoveConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 99);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("move constructing from a value")
	{
		int primitive_value{44};
		Maybe<int> dut_primitive(std::move(primitive_value));
		DefaultConstructible default_value{66};
		Maybe<DefaultConstructible> dut_default(std::move(default_value));
		ExplicitConstructible explicit_value{77};
		const Maybe<ExplicitConstructible> dut_explicit(std::move(explicit_value));
		static_assert(!std::is_constructible_v<Maybe<CopyableOnly>, CopyableOnly&&>);
		MoveableOnly moveable_only_value{99};
		Maybe<MoveableOnly> dut_move(std::move(moveable_only_value));
		static_assert(!std::is_constructible_v<Maybe<NonCopyableNonMovable>, NonCopyableNonMovable&&>);
		static_assert(!std::is_constructible_v<Maybe<CopyConstructibleOnly>, CopyConstructibleOnly&&>);
		MoveConstructibleOnly move_constructible_only_value{132};
		Maybe<MoveConstructibleOnly> dut_no_move_assign(std::move(move_constructible_only_value));

		THEN("the Maybes contain the value, the source is empty")
		{
			dut_primitive.match([](int val) { CHECK(val == 44); }, [](std::nullopt_t) { CHECK(false); });

			CHECK(default_value.m_method == Method::MovedFrom);
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 66);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });

			CHECK(explicit_value.m_method == Method::MovedFrom);
			dut_explicit.match(
				[](const ExplicitConstructible& wrap) {
					CHECK(wrap.m_value == 77);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			CHECK(moveable_only_value.m_method == Method::MovedFrom);
			dut_move.match(
				[](const MoveableOnly& wrap) {
					CHECK(wrap.m_value == 99);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			CHECK(move_constructible_only_value.m_method == Method::MovedFrom);
			dut_no_move_assign.match(
				[](const MoveConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 132);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("converting copy construction from an empty-Maybe")
	{
		static_assert(std::is_constructible_v<Maybe<DefaultConstructible>, const Maybe<int>&>);
		static_assert(!std::is_convertible_v<const Maybe<int>&, Maybe<DefaultConstructible>>);
		static_assert(std::is_constructible_v<Maybe<DefaultConstructible>, const Maybe<int>&>);
		static_assert(!std::is_convertible_v<const Maybe<int>&, Maybe<DefaultConstructible>>);
		static_assert(std::is_constructible_v<Maybe<ExplicitConstructible>, const Maybe<int>&>);
		static_assert(!std::is_convertible_v<const Maybe<int>&, Maybe<ExplicitConstructible>>);
		static_assert(std::is_constructible_v<Maybe<CopyableOnly>, const Maybe<int>&>);
		static_assert(!std::is_convertible_v<const Maybe<int>&, Maybe<CopyableOnly>>);
		static_assert(std::is_constructible_v<Maybe<MoveableOnly>, const Maybe<int>&>);
		static_assert(!std::is_convertible_v<const Maybe<int>&, Maybe<MoveableOnly>>);
		static_assert(std::is_constructible_v<Maybe<NonCopyableNonMovable>, const Maybe<int>&>);
		static_assert(!std::is_convertible_v<const Maybe<int>&, Maybe<NonCopyableNonMovable>>);
		static_assert(std::is_constructible_v<Maybe<CopyConstructibleOnly>, const Maybe<int>&>);
		static_assert(!std::is_convertible_v<const Maybe<int>&, Maybe<CopyConstructibleOnly>>);
		static_assert(std::is_constructible_v<Maybe<MoveConstructibleOnly>, const Maybe<int>&>);
		static_assert(!std::is_convertible_v<const Maybe<int>&, Maybe<MoveConstructibleOnly>>);

		static_assert(!std::is_constructible_v<Maybe<CopyableOnly>, Maybe<MoveableOnly>&>);
		static_assert(!std::is_constructible_v<Maybe<MoveableOnly>, Maybe<CopyableOnly>&>);

		Maybe<int> dut_primitive{std::nullopt};
		Maybe<DefaultConstructible> dut_default{dut_primitive};
		const Maybe<ExplicitConstructible> dut_explicit{dut_primitive};
		Maybe<CopyableOnly> dut_copy{dut_primitive};
		Maybe<MoveableOnly> dut_move{dut_primitive};
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move{dut_primitive};
		Maybe<CopyConstructibleOnly> dut_no_copy_assign{dut_primitive};
		Maybe<MoveConstructibleOnly> dut_no_move_assign{dut_primitive};

		THEN("the Maybes are empty")
		{
			CHECK_FALSE(dut_primitive.has_value());
			CHECK_FALSE(dut_default.has_value());
			CHECK_FALSE(dut_explicit.has_value());
			CHECK_FALSE(dut_copy.has_value());
			CHECK_FALSE(dut_move.has_value());
			CHECK_FALSE(dut_no_copy_no_move.has_value());
			CHECK_FALSE(dut_no_copy_assign.has_value());
			CHECK_FALSE(dut_no_move_assign.has_value());
		}
	}

	WHEN("converting copy construction from a non-empty-Maybe")
	{
		Maybe<int> dut_primitive{42};
		Maybe<DefaultConstructible> dut_default{dut_primitive};
		const Maybe<ExplicitConstructible> dut_explicit{dut_primitive};
		Maybe<CopyableOnly> dut_copy{dut_primitive};
		Maybe<MoveableOnly> dut_move{dut_primitive};
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move{dut_primitive};
		Maybe<CopyConstructibleOnly> dut_no_copy_assign{dut_primitive};
		Maybe<MoveConstructibleOnly> dut_no_move_assign{dut_primitive};

		THEN("the Maybes contains the value")
		{
			CHECK(dut_primitive.match([](int val) { return val == 42; }, [](std::nullopt_t) { return false; }));
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 42);
					// Note: for DefaultConstructible, default and value constructed are the same.
					CHECK(wrap.m_method == Method::DefaultConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit.match(
				[](const ExplicitConstructible& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_copy.match(
				[](const CopyableOnly& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_move.match(
				[](const MoveableOnly& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_copy_no_move.match(
				[](const NonCopyableNonMovable& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_copy_assign.match(
				[](const CopyConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_move_assign.match(
				[](const MoveConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("converting copy constructing from a value")
	{
		const char forty_two = '*';
		Maybe<int> dut_primitive{forty_two};
		Maybe<DefaultConstructible> dut_default{forty_two};
		const Maybe<ExplicitConstructible> dut_explicit{forty_two};
		Maybe<CopyableOnly> dut_copy{forty_two};
		Maybe<MoveableOnly> dut_move{forty_two};
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move{forty_two};
		Maybe<CopyConstructibleOnly> dut_no_copy_assign{forty_two};
		Maybe<MoveConstructibleOnly> dut_no_move_assign{forty_two};

		THEN("the Maybes contains the value")
		{
			dut_primitive.match([](int val) { CHECK(val == 42); }, [](std::nullopt_t) { CHECK(false); });
			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 42);
					// Note: for DefaultConstructible, default and value constructed are the same.
					CHECK(wrap.m_method == Method::DefaultConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit.match(
				[](const ExplicitConstructible& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_copy.match(
				[](const CopyableOnly& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_move.match(
				[](const MoveableOnly& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_copy_no_move.match(
				[](const NonCopyableNonMovable& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_copy_assign.match(
				[](const CopyConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_move_assign.match(
				[](const MoveConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("converting move constructing from an empty Maybe")
	{
		static_assert(std::is_constructible_v<Maybe<DefaultConstructible>, Maybe<int>&&>);
		static_assert(!std::is_convertible_v<Maybe<int>&&, Maybe<DefaultConstructible>>);
		static_assert(std::is_constructible_v<Maybe<ExplicitConstructible>, Maybe<int>&&>);
		static_assert(!std::is_convertible_v<Maybe<int>&&, Maybe<ExplicitConstructible>>);
		static_assert(std::is_constructible_v<Maybe<CopyableOnly>, Maybe<int>&&>);
		static_assert(!std::is_convertible_v<Maybe<int>&&, Maybe<CopyableOnly>>);
		static_assert(std::is_constructible_v<Maybe<MoveableOnly>, Maybe<int>&&>);
		static_assert(!std::is_convertible_v<Maybe<int>&&, Maybe<MoveableOnly>>);
		static_assert(std::is_constructible_v<Maybe<NonCopyableNonMovable>, Maybe<int>&&>);
		static_assert(!std::is_convertible_v<Maybe<int>&&, Maybe<NonCopyableNonMovable>>);
		static_assert(std::is_constructible_v<Maybe<CopyConstructibleOnly>, Maybe<int>&&>);
		static_assert(!std::is_convertible_v<Maybe<int>&&, Maybe<CopyConstructibleOnly>>);
		static_assert(std::is_constructible_v<Maybe<MoveConstructibleOnly>, Maybe<int>&&>);
		static_assert(!std::is_convertible_v<Maybe<int>&&, Maybe<MoveConstructibleOnly>>);

		static_assert(!std::is_constructible_v<Maybe<CopyableOnly>, Maybe<MoveableOnly>&&>);
		static_assert(!std::is_constructible_v<Maybe<MoveableOnly>, Maybe<CopyableOnly>&&>);

		Maybe<int> dut_default_empty{std::nullopt};
		Maybe<DefaultConstructible> dut_default{std::move(dut_default_empty)};
		Maybe<int> dut_explicit_empty{std::nullopt};
		const Maybe<ExplicitConstructible> dut_explicit{std::move(dut_explicit_empty)};
		Maybe<int> dut_copy_empty{std::nullopt};
		Maybe<CopyableOnly> dut_copy{std::move(dut_copy_empty)};
		Maybe<int> dut_move_empty{std::nullopt};
		Maybe<MoveableOnly> dut_move{std::move(dut_move_empty)};
		Maybe<int> dut_no_copy_no_move_empty{std::nullopt};
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move{std::move(dut_no_copy_no_move_empty)};
		Maybe<int> dut_no_copy_assign_empty{std::nullopt};
		Maybe<CopyConstructibleOnly> dut_no_copy_assign{std::move(dut_no_copy_assign_empty)};
		Maybe<int> dut_no_move_assign_empty{std::nullopt};
		Maybe<MoveConstructibleOnly> dut_no_move_assign{std::move(dut_no_move_assign_empty)};

		THEN("all Maybes are empty")
		{
			CHECK_FALSE(dut_default.has_value());
			CHECK_FALSE(dut_explicit.has_value());
			CHECK_FALSE(dut_copy.has_value());
			CHECK_FALSE(dut_move.has_value());
			CHECK_FALSE(dut_no_copy_no_move.has_value());
			CHECK_FALSE(dut_no_copy_assign.has_value());
			CHECK_FALSE(dut_no_move_assign.has_value());
		}
	}

	WHEN("converting move constructing from a non-empty Maybe")
	{
		Maybe<int> dut_default_value{33};
		Maybe<DefaultConstructible> dut_default{std::move(dut_default_value)};
		Maybe<int> dut_explicit_value{42};
		Maybe<ExplicitConstructible> dut_explicit{std::move(dut_explicit_value)};
		Maybe<int> dut_copy_value{51};
		Maybe<CopyableOnly> dut_copy{std::move(dut_copy_value)};
		Maybe<int> dut_move_value{61};
		Maybe<MoveableOnly> dut_move{std::move(dut_move_value)};
		Maybe<int> dut_no_copy_no_move_value{71};
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move{std::move(dut_no_copy_no_move_value)};
		Maybe<int> dut_no_copy_assign_value{81};
		Maybe<CopyConstructibleOnly> dut_no_copy_assign{std::move(dut_no_copy_assign_value)};
		Maybe<int> dut_no_move_assign_value{91};
		Maybe<MoveConstructibleOnly> dut_no_move_assign{std::move(dut_no_move_assign_value)};

		THEN("the Maybes contain the value, the sources are empty")
		{
			CHECK_FALSE(dut_default_value.has_value());
			CHECK_FALSE(dut_explicit_value.has_value());
			CHECK_FALSE(dut_copy_value.has_value());
			CHECK_FALSE(dut_move_value.has_value());
			CHECK_FALSE(dut_no_copy_no_move_value.has_value());
			CHECK_FALSE(dut_no_copy_assign_value.has_value());
			CHECK_FALSE(dut_no_move_assign_value.has_value());

			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 33);
					CHECK(wrap.m_method == Method::DefaultConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_explicit.match(
				[](const ExplicitConstructible& wrap) {
					CHECK(wrap.m_value == 42);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_copy.match(
				[](const CopyableOnly& wrap) {
					CHECK(wrap.m_value == 51);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_move.match(
				[](const MoveableOnly& wrap) {
					CHECK(wrap.m_value == 61);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_copy_no_move.match(
				[](const NonCopyableNonMovable& wrap) {
					CHECK(wrap.m_value == 71);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_copy_assign.match(
				[](const CopyConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 81);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
			dut_no_move_assign.match(
				[](const MoveConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 91);
					CHECK(wrap.m_method == Method::ValueConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("converting move constructing from a value")
	{
		DefaultConstructible default_value{66};
		Maybe<DefaultConstructible> dut_default(std::move(default_value));
		ExplicitConstructible explicit_value{77};
		const Maybe<ExplicitConstructible> dut_explicit(std::move(explicit_value));
		static_assert(!std::is_constructible_v<Maybe<CopyableOnly>, CopyableOnly&&>);
		MoveableOnly moveable_only_value{99};
		Maybe<MoveableOnly> dut_move(std::move(moveable_only_value));
		static_assert(!std::is_constructible_v<Maybe<NonCopyableNonMovable>, NonCopyableNonMovable&&>);
		static_assert(!std::is_constructible_v<Maybe<CopyConstructibleOnly>, CopyConstructibleOnly&&>);
		MoveConstructibleOnly move_constructible_only_value{132};
		Maybe<MoveConstructibleOnly> dut_no_move_assign(std::move(move_constructible_only_value));

		THEN("the Maybes contain the value, the source is empty")
		{
			CHECK(default_value.m_method == Method::MovedFrom);
			CHECK(explicit_value.m_method == Method::MovedFrom);
			CHECK(moveable_only_value.m_method == Method::MovedFrom);
			CHECK(move_constructible_only_value.m_method == Method::MovedFrom);

			dut_default.match(
				[](const DefaultConstructible& wrap) {
					CHECK(wrap.m_value == 66);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });

			dut_explicit.match(
				[](const ExplicitConstructible& wrap) {
					CHECK(wrap.m_value == 77);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });

			dut_move.match(
				[](const MoveableOnly& wrap) {
					CHECK(wrap.m_value == 99);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });

			dut_no_move_assign.match(
				[](const MoveConstructibleOnly& wrap) {
					CHECK(wrap.m_value == 132);
					CHECK(wrap.m_method == Method::MoveConstructed);
				},
				[](std::nullopt_t) { CHECK(false); });
		}
	}

	WHEN("in-place construction")
	{
		using pair_t = std::pair<char, int>;
		static_assert(std::is_constructible_v<Maybe<pair_t>, std::in_place_t, char, int>);
		static_assert(!std::is_constructible_v<Maybe<ExplicitConstructible>, std::in_place_t>);
		static_assert(!std::is_constructible_v<Maybe<pair_t>, std::in_place_t, char>);
		static_assert(!std::is_constructible_v<Maybe<pair_t>, std::in_place_t, char, int, int>);

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
		static_assert(std::is_constructible_v<Maybe<vec_t>, std::in_place_t, std::initializer_list<int>>);
		static_assert(!std::is_constructible_v<Maybe<vec_t>, std::in_place_t, std::initializer_list<char>, int>);

		Maybe<vec_t> dut(std::in_place, {1, 2, 3, 4, 5});

		static_assert(std::is_constructible_v<Maybe<NonMovableList>, std::in_place_t, std::initializer_list<int>>);
		Maybe<NonMovableList> non_movable(std::in_place, {1, 2, 3});

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
			CHECK(non_movable.match([](const NonMovableList& value) { return value.m_size == 3; },
									[](std::nullopt_t) { return false; }));
		}
	}
}

SCENARIO("Maybe: assignment")
{
	GIVEN("an empty Maybe")
	{
		Maybe<int> dut_primitive_empty;
		Maybe<DefaultConstructible> dut_default_empty;
		Maybe<ExplicitConstructible> dut_explicit_empty;
		Maybe<CopyableOnly> dut_copy_empty;
		Maybe<MoveableOnly> dut_move_empty;
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move_empty;
		Maybe<CopyConstructibleOnly> dut_no_copy_assign_empty;
		Maybe<MoveConstructibleOnly> dut_no_move_assign_empty;

		WHEN("nullopt assignment")
		{
			dut_primitive_empty = std::nullopt;
			dut_default_empty = std::nullopt;
			dut_explicit_empty = std::nullopt;
			dut_copy_empty = std::nullopt;
			dut_move_empty = std::nullopt;
			dut_no_copy_no_move_empty = std::nullopt;
			dut_no_copy_assign_empty = std::nullopt;
			dut_no_move_assign_empty = std::nullopt;

			THEN("all Maybes are empty")
			{
				CHECK_FALSE(dut_primitive_empty.has_value());
				CHECK_FALSE(dut_default_empty.has_value());
				CHECK_FALSE(dut_explicit_empty.has_value());
				CHECK_FALSE(dut_copy_empty.has_value());
				CHECK_FALSE(dut_move_empty.has_value());
				CHECK_FALSE(dut_no_copy_no_move_empty.has_value());
				CHECK_FALSE(dut_no_copy_assign_empty.has_value());
				CHECK_FALSE(dut_no_move_assign_empty.has_value());
			}
		}

		WHEN("value assignment")
		{
			DefaultConstructible default_value_rhs{12};
			ExplicitConstructible explicit_value_rhs{42};
			CopyableOnly copy_value_rhs{4};

			dut_primitive_empty = 1;
			dut_default_empty = default_value_rhs;
			dut_explicit_empty = explicit_value_rhs;
			dut_copy_empty = copy_value_rhs;
			static_assert(!std::is_assignable_v<Maybe<MoveableOnly>&, MoveableOnly&>);
			static_assert(std::is_assignable_v<Maybe<MoveableOnly>&, MoveableOnly&&>);
			static_assert(!std::is_assignable_v<Maybe<MoveableOnly>&, const MoveableOnly&&>);
			static_assert(!std::is_assignable_v<Maybe<NonCopyableNonMovable>&, NonCopyableNonMovable&>);
			static_assert(!std::is_assignable_v<Maybe<CopyConstructibleOnly>&, CopyConstructibleOnly&>);
			static_assert(!std::is_assignable_v<Maybe<MoveConstructibleOnly>&, MoveConstructibleOnly&>);

			THEN("the Maybes are engaged and contain the assigned values")
			{
				CHECK(dut_primitive_empty.has_value());
				dut_primitive_empty.match([](int val) { CHECK(val == 1); }, [](std::nullopt_t) { CHECK(false); });

				CHECK(dut_default_empty.has_value());
				dut_default_empty.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 12);
						CHECK(wrap.m_method == Method::CopyConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(dut_explicit_empty.has_value());
				dut_explicit_empty.match(
					[](const ExplicitConstructible& wrap) {
						CHECK(wrap.m_value == 42);
						CHECK(wrap.m_method == Method::CopyConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(dut_copy_empty.has_value());
				dut_copy_empty.match(
					[](const CopyableOnly& wrap) {
						CHECK(wrap.m_value == 4);
						CHECK(wrap.m_method == Method::CopyConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
			}
		}

		WHEN("value assignment (move)")
		{
			int primitive_value_rhs(31);
			DefaultConstructible default_value_rhs(33);
			ExplicitConstructible explicit_value_rhs(35);
			MoveableOnly move_value_rhs(37);

			dut_primitive_empty = std::move(primitive_value_rhs);
			dut_default_empty = std::move(default_value_rhs);
			dut_explicit_empty = std::move(explicit_value_rhs);
			static_assert(!std::is_move_assignable_v<Maybe<CopyableOnly>>);
			dut_move_empty = std::move(move_value_rhs);
			static_assert(!std::is_move_assignable_v<Maybe<NonCopyableNonMovable>>);
			static_assert(!std::is_move_assignable_v<Maybe<CopyConstructibleOnly>>);
			static_assert(!std::is_move_assignable_v<Maybe<MoveConstructibleOnly>>);

			THEN("the Maybes are engaged and contain the assigned values")
			{
				CHECK(dut_primitive_empty.has_value());
				dut_primitive_empty.match([](int val) { CHECK(val == 31); }, [](std::nullopt_t) { CHECK(false); });

				CHECK(dut_default_empty.has_value());
				dut_default_empty.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(dut_explicit_empty.has_value());
				dut_explicit_empty.match(
					[](const ExplicitConstructible& wrap) {
						CHECK(wrap.m_value == 35);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(dut_move_empty.has_value());
				dut_move_empty.match(
					[](const MoveableOnly& wrap) {
						CHECK(wrap.m_value == 37);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(default_value_rhs.m_method == Method::MovedFrom);
				CHECK(explicit_value_rhs.m_method == Method::MovedFrom);
				CHECK(move_value_rhs.m_method == Method::MovedFrom);
			}
		}

		WHEN("copy assignment with an empty Maybe")
		{
			Maybe<int> primitive_empty;
			Maybe<DefaultConstructible> default_empty;
			Maybe<ExplicitConstructible> explicit_empty;
			Maybe<CopyableOnly> copy_empty;

			dut_primitive_empty = primitive_empty;
			dut_default_empty = default_empty;
			dut_explicit_empty = explicit_empty;
			dut_copy_empty = copy_empty;
			static_assert(!std::is_copy_assignable_v<Maybe<MoveableOnly>>);
			static_assert(!std::is_copy_assignable_v<Maybe<NonCopyableNonMovable>>);
			static_assert(!std::is_copy_assignable_v<Maybe<CopyConstructibleOnly>>);
			static_assert(!std::is_copy_assignable_v<Maybe<MoveConstructibleOnly>>);

			THEN("all Maybes are empty")
			{
				CHECK_FALSE(primitive_empty.has_value());
				CHECK_FALSE(default_empty.has_value());
				CHECK_FALSE(explicit_empty.has_value());
				CHECK_FALSE(copy_empty.has_value());

				CHECK_FALSE(dut_primitive_empty.has_value());
				CHECK_FALSE(dut_default_empty.has_value());
				CHECK_FALSE(dut_explicit_empty.has_value());
				CHECK_FALSE(dut_copy_empty.has_value());
			}
		}

		WHEN("copy assignment with a engaged Maybe")
		{
			const Maybe<int> primitive_value(31);
			const Maybe<DefaultConstructible> default_value(32);
			const Maybe<ExplicitConstructible> explicit_value(33);
			const Maybe<CopyableOnly> copy_value(34);

			dut_primitive_empty = primitive_value;
			dut_default_empty = default_value;
			dut_explicit_empty = explicit_value;
			dut_copy_empty = copy_value;
			static_assert(!std::is_copy_assignable_v<Maybe<MoveableOnly>>);
			static_assert(!std::is_copy_assignable_v<Maybe<NonCopyableNonMovable>>);
			static_assert(!std::is_copy_assignable_v<Maybe<CopyConstructibleOnly>>);
			static_assert(!std::is_copy_assignable_v<Maybe<MoveConstructibleOnly>>);

			THEN("the Maybes are engaged and contain the assigned value")
			{
				dut_primitive_empty.match([](int val) { CHECK(val == 31); }, [](std::nullopt_t) { CHECK(false); });
				primitive_value.match([](int val) { CHECK(val == 31); }, [](std::nullopt_t) { CHECK(false); });

				CHECK(default_value.has_value());
				dut_default_empty.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 32);
						CHECK(wrap.m_method == Method::CopyConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(explicit_value.has_value());
				dut_explicit_empty.match(
					[](const ExplicitConstructible& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::CopyConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(copy_value.has_value());
				dut_copy_empty.match(
					[](const CopyableOnly& wrap) {
						CHECK(wrap.m_value == 34);
						CHECK(wrap.m_method == Method::CopyConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
			}
		}

		WHEN("move assignment with an empty Maybe")
		{
			Maybe<int> primitive_empty;
			Maybe<DefaultConstructible> default_empty;
			Maybe<ExplicitConstructible> explicit_empty;
			Maybe<MoveableOnly> move_empty;

			dut_primitive_empty = std::move(primitive_empty);
			dut_default_empty = std::move(default_empty);
			dut_explicit_empty = std::move(explicit_empty);
			static_assert(!std::is_move_assignable_v<Maybe<CopyableOnly>>);
			dut_move_empty = std::move(move_empty);
			static_assert(!std::is_move_assignable_v<Maybe<NonCopyableNonMovable>>);
			static_assert(!std::is_move_assignable_v<Maybe<CopyConstructibleOnly>>);
			static_assert(!std::is_move_assignable_v<Maybe<MoveConstructibleOnly>>);

			THEN("all Maybes are empty")
			{
				CHECK_FALSE(primitive_empty.has_value());
				CHECK_FALSE(default_empty.has_value());
				CHECK_FALSE(explicit_empty.has_value());
				CHECK_FALSE(move_empty.has_value());

				CHECK_FALSE(dut_primitive_empty.has_value());
				CHECK_FALSE(dut_default_empty.has_value());
				CHECK_FALSE(dut_explicit_empty.has_value());
				CHECK_FALSE(dut_move_empty.has_value());
			}
		}

		WHEN("move assignment from a engaged Maybe")
		{
			Maybe<int> primitive_value(31);
			Maybe<DefaultConstructible> default_value(33);
			Maybe<ExplicitConstructible> explicit_value(35);
			Maybe<MoveableOnly> move_value(37);

			dut_primitive_empty = std::move(primitive_value);
			dut_default_empty = std::move(default_value);
			dut_explicit_empty = std::move(explicit_value);
			static_assert(!std::is_move_assignable_v<Maybe<CopyableOnly>>);
			dut_move_empty = std::move(move_value);
			static_assert(!std::is_move_assignable_v<Maybe<NonCopyableNonMovable>>);
			static_assert(!std::is_move_assignable_v<Maybe<CopyConstructibleOnly>>);
			static_assert(!std::is_move_assignable_v<Maybe<MoveConstructibleOnly>>);

			THEN("lhs is engaged and contains the value and rhs is empty")
			{
				CHECK_FALSE(primitive_value.has_value());
				CHECK_FALSE(default_value.has_value());
				CHECK_FALSE(explicit_value.has_value());
				CHECK_FALSE(move_value.has_value());

				dut_primitive_empty.match([](int val) { CHECK(val == 31); }, [](std::nullopt_t) { CHECK(false); });

				dut_default_empty.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });

				dut_explicit_empty.match(
					[](const ExplicitConstructible& wrap) {
						CHECK(wrap.m_value == 35);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });

				dut_move_empty.match(
					[](const MoveableOnly& wrap) {
						CHECK(wrap.m_value == 37);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
			}
		}

		WHEN("emplace (value)")
		{
			const auto result1 = dut_primitive_empty.emplace(41);
			const auto result2 = dut_default_empty.emplace(42);
			const auto& result3 = dut_explicit_empty.emplace(40);
			const auto& result4 = dut_copy_empty.emplace(102);
			const auto& result5 = dut_move_empty.emplace(96);
			const auto& result6 = dut_no_copy_no_move_empty.emplace(87);
			const auto& result7 = dut_no_copy_assign_empty.emplace(78);
			const auto& result8 = dut_no_move_assign_empty.emplace(69);

			THEN("the Maybes contain the value")
			{
				CHECK(dut_primitive_empty.has_value());
				dut_primitive_empty.match([](int val) { CHECK(val == 41); }, [](std::nullopt_t) { CHECK(false); });
				CHECK(result1 == 41);

				CHECK(dut_default_empty.has_value());
				dut_default_empty.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 42);
						CHECK(wrap.m_method == Method::DefaultConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result2.m_value == 42);

				CHECK(dut_explicit_empty.has_value());
				dut_explicit_empty.match(
					[](const ExplicitConstructible& wrap) {
						CHECK(wrap.m_value == 40);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result3.m_value == 40);

				CHECK(dut_copy_empty.has_value());
				dut_copy_empty.match(
					[](const CopyableOnly& wrap) {
						CHECK(wrap.m_value == 102);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result4.m_value == 102);

				CHECK(dut_move_empty.has_value());
				dut_move_empty.match(
					[](const MoveableOnly& wrap) {
						CHECK(wrap.m_value == 96);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result5.m_value == 96);

				CHECK(dut_no_copy_no_move_empty.has_value());
				dut_no_copy_no_move_empty.match(
					[](const NonCopyableNonMovable& wrap) {
						CHECK(wrap.m_value == 87);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result6.m_value == 87);

				CHECK(dut_no_copy_assign_empty.has_value());
				dut_no_copy_assign_empty.match(
					[](const CopyConstructibleOnly& wrap) {
						CHECK(wrap.m_value == 78);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result7.m_value == 78);

				CHECK(dut_no_move_assign_empty.has_value());
				dut_no_move_assign_empty.match(
					[](const MoveConstructibleOnly& wrap) {
						CHECK(wrap.m_value == 69);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result8.m_value == 69);
			}
		}

		WHEN("emplace (move)")
		{
			int primitive_value = 41;
			const auto result1 = dut_primitive_empty.emplace(std::move(primitive_value));
			DefaultConstructible default_value(42);
			const auto result2 = dut_default_empty.emplace(std::move(default_value));
			ExplicitConstructible explicit_value(40);
			const auto& result3 = dut_explicit_empty.emplace(std::move(explicit_value));
			static_assert(!std::is_move_constructible_v<CopyableOnly>);
			MoveableOnly move_value(96);
			const auto& result5 = dut_move_empty.emplace(std::move(move_value));
			static_assert(!std::is_move_constructible_v<NonCopyableNonMovable>);
			static_assert(!std::is_move_constructible_v<CopyConstructibleOnly>);
			MoveConstructibleOnly no_move_assign_value(69);
			const auto& result8 = dut_no_move_assign_empty.emplace(std::move(no_move_assign_value));

			THEN("the Maybes contain the value")
			{
				CHECK(dut_primitive_empty.has_value());
				dut_primitive_empty.match([](int val) { CHECK(val == 41); }, [](std::nullopt_t) { CHECK(false); });
				CHECK(result1 == 41);

				CHECK(dut_default_empty.has_value());
				dut_default_empty.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 42);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result2.m_value == 42);

				CHECK(dut_explicit_empty.has_value());
				dut_explicit_empty.match(
					[](const ExplicitConstructible& wrap) {
						CHECK(wrap.m_value == 40);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result3.m_value == 40);

				CHECK(dut_move_empty.has_value());
				dut_move_empty.match(
					[](const MoveableOnly& wrap) {
						CHECK(wrap.m_value == 96);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result5.m_value == 96);

				CHECK(dut_no_move_assign_empty.has_value());
				dut_no_move_assign_empty.match(
					[](const MoveConstructibleOnly& wrap) {
						CHECK(wrap.m_value == 69);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result8.m_value == 69);
			}
		}
	}

	GIVEN("an engaged Maybe")
	{
		Maybe<int> dut_primitive_value(42);
		Maybe<DefaultConstructible> dut_default_value(33);
		Maybe<ExplicitConstructible> dut_explicit_value(24);
		Maybe<CopyableOnly> dut_copy_value(15);
		Maybe<MoveableOnly> dut_move_value(96);
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move_value(87);
		Maybe<CopyConstructibleOnly> dut_no_copy_assign_value(78);
		Maybe<MoveConstructibleOnly> dut_no_move_assign_value(69);

		WHEN("nullopt assignment")
		{
			dut_primitive_value = std::nullopt;
			dut_default_value = std::nullopt;
			dut_explicit_value = std::nullopt;
			dut_copy_value = std::nullopt;
			dut_move_value = std::nullopt;
			dut_no_copy_no_move_value = std::nullopt;
			dut_no_copy_assign_value = std::nullopt;
			dut_no_move_assign_value = std::nullopt;

			THEN("all Maybes are empty")
			{
				CHECK_FALSE(dut_primitive_value.has_value());
				CHECK_FALSE(dut_default_value.has_value());
				CHECK_FALSE(dut_explicit_value.has_value());
				CHECK_FALSE(dut_copy_value.has_value());
				CHECK_FALSE(dut_move_value.has_value());
				CHECK_FALSE(dut_no_copy_no_move_value.has_value());
				CHECK_FALSE(dut_no_copy_assign_value.has_value());
				CHECK_FALSE(dut_no_move_assign_value.has_value());
			}
		}

		WHEN("value assignment")
		{
			static_assert(std::is_assignable_v<Maybe<ExplicitConstructible>&, ExplicitConstructible&>);
			static_assert(std::is_assignable_v<Maybe<ExplicitConstructible>&, const ExplicitConstructible&>);
			static_assert(std::is_assignable_v<Maybe<ExplicitConstructible>&, ExplicitConstructible&&>);
			static_assert(std::is_assignable_v<Maybe<ExplicitConstructible>&, const ExplicitConstructible&&>);
			static_assert(!std::is_assignable_v<Maybe<ExplicitConstructible>&, int>);
			static_assert(!std::is_convertible_v<int, Maybe<ExplicitConstructible>>);

			DefaultConstructible default_value_rhs{12};
			ExplicitConstructible explicit_value_rhs{42};
			CopyableOnly copy_value_rhs{4};

			dut_primitive_value = 2;
			dut_default_value = default_value_rhs;
			dut_explicit_value = explicit_value_rhs;
			dut_copy_value = copy_value_rhs;
			static_assert(!std::is_assignable_v<Maybe<MoveableOnly>&, MoveableOnly&>);
			static_assert(std::is_assignable_v<Maybe<MoveableOnly>&, MoveableOnly&&>);
			static_assert(!std::is_assignable_v<Maybe<MoveableOnly>&, const MoveableOnly&&>);
			static_assert(!std::is_assignable_v<Maybe<NonCopyableNonMovable>&, NonCopyableNonMovable&>);
			static_assert(!std::is_assignable_v<Maybe<CopyConstructibleOnly>&, CopyConstructibleOnly&>);
			static_assert(!std::is_assignable_v<Maybe<MoveConstructibleOnly>&, MoveConstructibleOnly&>);

			THEN("the Maybes are engaged and contain the assigned values")
			{
				CHECK(dut_primitive_value.has_value());
				dut_primitive_value.match([](int val) { CHECK(val == 2); }, [](std::nullopt_t) { CHECK(false); });

				CHECK(dut_default_value.has_value());
				dut_default_value.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 12);
						CHECK(wrap.m_method == Method::CopyAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(dut_explicit_value.has_value());
				dut_explicit_value.match(
					[](const ExplicitConstructible& wrap) {
						CHECK(wrap.m_value == 42);
						CHECK(wrap.m_method == Method::CopyAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(dut_copy_value.has_value());
				dut_copy_value.match(
					[](const CopyableOnly& wrap) {
						CHECK(wrap.m_value == 4);
						CHECK(wrap.m_method == Method::CopyAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });
			}
		}

		WHEN("value assignment (move)")
		{
			static_assert(std::is_assignable_v<Maybe<DefaultConstructible>&, DefaultConstructible&>);
			static_assert(std::is_assignable_v<Maybe<DefaultConstructible>&, const DefaultConstructible&>);
			static_assert(std::is_assignable_v<Maybe<DefaultConstructible>&, DefaultConstructible&&>);
			static_assert(std::is_assignable_v<Maybe<DefaultConstructible>&, const DefaultConstructible&&>);

			int primitive_value_rhs(32);
			DefaultConstructible default_value_rhs(34);
			ExplicitConstructible explicit_value_rhs(36);
			MoveableOnly move_value_rhs(38);

			dut_primitive_value = std::move(primitive_value_rhs);
			dut_default_value = std::move(default_value_rhs);
			dut_explicit_value = std::move(explicit_value_rhs);
			static_assert(!std::is_move_assignable_v<Maybe<CopyableOnly>>);
			dut_move_value = std::move(move_value_rhs);
			static_assert(!std::is_move_assignable_v<Maybe<NonCopyableNonMovable>>);
			static_assert(!std::is_move_assignable_v<Maybe<CopyConstructibleOnly>>);
			static_assert(!std::is_move_assignable_v<Maybe<MoveConstructibleOnly>>);

			THEN("the Maybes are engaged and contain the assigned values")
			{
				CHECK(dut_primitive_value.has_value());
				dut_primitive_value.match([](int val) { CHECK(val == 32); }, [](std::nullopt_t) { CHECK(false); });

				CHECK(dut_default_value.has_value());
				dut_default_value.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 34);
						CHECK(wrap.m_method == Method::MoveAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(dut_explicit_value.has_value());
				dut_explicit_value.match(
					[](const ExplicitConstructible& wrap) {
						CHECK(wrap.m_value == 36);
						CHECK(wrap.m_method == Method::MoveAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(dut_move_value.has_value());
				dut_move_value.match(
					[](const MoveableOnly& wrap) {
						CHECK(wrap.m_value == 38);
						CHECK(wrap.m_method == Method::MoveAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(default_value_rhs.m_method == Method::MovedFrom);
				CHECK(explicit_value_rhs.m_method == Method::MovedFrom);
				CHECK(move_value_rhs.m_method == Method::MovedFrom);
			}
		}

		WHEN("copy assignment with an empty Maybe")
		{
			Maybe<int> primitive_empty;
			Maybe<DefaultConstructible> default_empty;
			Maybe<ExplicitConstructible> explicit_empty;
			Maybe<CopyableOnly> copy_empty;

			dut_primitive_value = primitive_empty;
			dut_default_value = default_empty;
			dut_explicit_value = explicit_empty;
			dut_copy_value = copy_empty;
			static_assert(!std::is_copy_assignable_v<Maybe<MoveableOnly>>);
			static_assert(!std::is_copy_assignable_v<Maybe<NonCopyableNonMovable>>);
			static_assert(!std::is_copy_assignable_v<Maybe<CopyConstructibleOnly>>);
			static_assert(!std::is_copy_assignable_v<Maybe<MoveConstructibleOnly>>);

			THEN("all Maybes are empty")
			{
				CHECK_FALSE(primitive_empty.has_value());
				CHECK_FALSE(default_empty.has_value());
				CHECK_FALSE(explicit_empty.has_value());
				CHECK_FALSE(copy_empty.has_value());

				CHECK_FALSE(dut_primitive_value.has_value());
				CHECK_FALSE(dut_default_value.has_value());
				CHECK_FALSE(dut_explicit_value.has_value());
				CHECK_FALSE(dut_copy_value.has_value());
			}
		}

		WHEN("copy assignment with a engaged Maybe")
		{
			const Maybe<int> primitive_value(31);
			const Maybe<DefaultConstructible> default_value(32);
			const Maybe<ExplicitConstructible> explicit_value(33);
			const Maybe<CopyableOnly> copy_value(34);

			dut_primitive_value = primitive_value;
			dut_default_value = default_value;
			dut_explicit_value = explicit_value;
			dut_copy_value = copy_value;
			static_assert(!std::is_copy_assignable_v<Maybe<MoveableOnly>>);
			static_assert(!std::is_copy_assignable_v<Maybe<NonCopyableNonMovable>>);
			static_assert(!std::is_copy_assignable_v<Maybe<CopyConstructibleOnly>>);
			static_assert(!std::is_copy_assignable_v<Maybe<MoveConstructibleOnly>>);

			THEN("the Maybes are engaged and contain the assigned value")
			{
				dut_primitive_value.match([](int val) { CHECK(val == 31); }, [](std::nullopt_t) { CHECK(false); });
				primitive_value.match([](int val) { CHECK(val == 31); }, [](std::nullopt_t) { CHECK(false); });

				CHECK(default_value.has_value());
				dut_default_value.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 32);
						CHECK(wrap.m_method == Method::CopyAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(explicit_value.has_value());
				dut_explicit_value.match(
					[](const ExplicitConstructible& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::CopyAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });

				CHECK(copy_value.has_value());
				dut_copy_value.match(
					[](const CopyableOnly& wrap) {
						CHECK(wrap.m_value == 34);
						CHECK(wrap.m_method == Method::CopyAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });
			}
		}

		WHEN("self-assignment via copy assignment")
		{
			Maybe<ExplicitConstructible> dut_explicit(ExplicitConstructible{42});
#pragma GCC diagnostic push
#if BOB_COMPILER_CLANG
#pragma GCC diagnostic ignored "-Wself-assign-overloaded"
#endif
			dut_explicit = dut_explicit;
#pragma GCC diagnostic pop

			THEN("the Maybe is still engaged and contains the same value")
			{
				CHECK(dut_explicit.has_value());
				dut_explicit.match([](const ExplicitConstructible& wrap) { CHECK(wrap.m_value == 42); },
								   [](std::nullopt_t) { CHECK(false); });
			}
		}

		WHEN("move assignment with an empty Maybe")
		{
			Maybe<int> primitive_empty;
			Maybe<DefaultConstructible> default_empty;
			Maybe<ExplicitConstructible> explicit_empty;
			Maybe<MoveableOnly> move_empty;

			dut_primitive_value = std::move(primitive_empty);
			static_assert(!std::is_move_assignable_v<Maybe<CopyableOnly>>);
			dut_default_value = std::move(default_empty);
			dut_explicit_value = std::move(explicit_empty);
			dut_move_value = std::move(move_empty);
			static_assert(!std::is_move_assignable_v<Maybe<NonCopyableNonMovable>>);
			static_assert(!std::is_move_assignable_v<Maybe<CopyConstructibleOnly>>);
			static_assert(!std::is_move_assignable_v<Maybe<MoveConstructibleOnly>>);

			THEN("all Maybes are empty")
			{
				CHECK_FALSE(primitive_empty.has_value());
				CHECK_FALSE(default_empty.has_value());
				CHECK_FALSE(explicit_empty.has_value());
				CHECK_FALSE(move_empty.has_value());

				CHECK_FALSE(dut_primitive_value.has_value());
				CHECK_FALSE(dut_default_value.has_value());
				CHECK_FALSE(dut_explicit_value.has_value());
				CHECK_FALSE(dut_move_value.has_value());
			}
		}

		WHEN("move assignment from a engaged Maybe")
		{
			Maybe<int> primitive_value(31);
			Maybe<DefaultConstructible> default_value(33);
			Maybe<ExplicitConstructible> explicit_value(35);
			Maybe<MoveableOnly> move_value(37);

			dut_primitive_value = std::move(primitive_value);
			dut_default_value = std::move(default_value);
			dut_explicit_value = std::move(explicit_value);
			static_assert(!std::is_move_assignable_v<Maybe<CopyableOnly>>);
			dut_move_value = std::move(move_value);
			static_assert(!std::is_move_assignable_v<Maybe<NonCopyableNonMovable>>);
			static_assert(!std::is_move_assignable_v<Maybe<CopyConstructibleOnly>>);
			static_assert(!std::is_move_assignable_v<Maybe<MoveConstructibleOnly>>);

			THEN("lhs is engaged and contains the value and rhs is empty")
			{
				CHECK_FALSE(primitive_value.has_value());
				CHECK_FALSE(default_value.has_value());
				CHECK_FALSE(explicit_value.has_value());
				CHECK_FALSE(move_value.has_value());

				dut_primitive_value.match([](int val) { CHECK(val == 31); }, [](std::nullopt_t) { CHECK(false); });

				dut_default_value.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 33);
						CHECK(wrap.m_method == Method::MoveAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });

				dut_explicit_value.match(
					[](const ExplicitConstructible& wrap) {
						CHECK(wrap.m_value == 35);
						CHECK(wrap.m_method == Method::MoveAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });

				dut_move_value.match(
					[](const MoveableOnly& wrap) {
						CHECK(wrap.m_value == 37);
						CHECK(wrap.m_method == Method::MoveAssigned);
					},
					[](std::nullopt_t) { CHECK(false); });
			}
		}

		WHEN("self-assignment via move assignment")
		{
			Maybe<DefaultConstructible> dut_default(DefaultConstructible{41});

#pragma GCC diagnostic push
#if BOB_COMPILER_CLANG || (BOB_COMPILER_GCC && __GNUC__ >= 13)
#pragma GCC diagnostic ignored "-Wself-move"
#endif
			dut_default = std::move(dut_default);
#pragma GCC diagnostic pop

			THEN("the Maybe is still engaged and contains the same value")
			{
				CHECK(dut_default.has_value());
				dut_default.match([](const DefaultConstructible& wrap) { CHECK(wrap.m_value == 41); },
								  [](std::nullopt_t) { CHECK(false); });
			}
		}

		WHEN("emplace (value)")
		{
			const auto result1 = dut_primitive_value.emplace(41);
			const auto result2 = dut_default_value.emplace(42);
			const auto& result3 = dut_explicit_value.emplace(40);
			const auto& result4 = dut_copy_value.emplace(102);
			const auto& result5 = dut_move_value.emplace(96);
			const auto& result6 = dut_no_copy_no_move_value.emplace(87);
			const auto& result7 = dut_no_copy_assign_value.emplace(78);
			const auto& result8 = dut_no_move_assign_value.emplace(69);

			THEN("the Maybes contain the value")
			{
				CHECK(dut_primitive_value.has_value());
				dut_primitive_value.match([](int val) { CHECK(val == 41); }, [](std::nullopt_t) { CHECK(false); });
				CHECK(result1 == 41);

				CHECK(dut_default_value.has_value());
				dut_default_value.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 42);
						CHECK(wrap.m_method == Method::DefaultConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result2.m_value == 42);

				CHECK(dut_explicit_value.has_value());
				dut_explicit_value.match(
					[](const ExplicitConstructible& wrap) {
						CHECK(wrap.m_value == 40);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result3.m_value == 40);

				CHECK(dut_copy_value.has_value());
				dut_copy_value.match(
					[](const CopyableOnly& wrap) {
						CHECK(wrap.m_value == 102);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result4.m_value == 102);

				CHECK(dut_move_value.has_value());
				dut_move_value.match(
					[](const MoveableOnly& wrap) {
						CHECK(wrap.m_value == 96);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result5.m_value == 96);

				CHECK(dut_no_copy_no_move_value.has_value());
				dut_no_copy_no_move_value.match(
					[](const NonCopyableNonMovable& wrap) {
						CHECK(wrap.m_value == 87);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result6.m_value == 87);

				CHECK(dut_no_copy_assign_value.has_value());
				dut_no_copy_assign_value.match(
					[](const CopyConstructibleOnly& wrap) {
						CHECK(wrap.m_value == 78);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result7.m_value == 78);

				CHECK(dut_no_move_assign_value.has_value());
				dut_no_move_assign_value.match(
					[](const MoveConstructibleOnly& wrap) {
						CHECK(wrap.m_value == 69);
						CHECK(wrap.m_method == Method::ValueConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result8.m_value == 69);
			}
		}

		WHEN("emplace (move)")
		{
			int primitive_value = 41;
			const auto result1 = dut_primitive_value.emplace(std::move(primitive_value));
			DefaultConstructible default_value(42);
			const auto result2 = dut_default_value.emplace(std::move(default_value));
			ExplicitConstructible explicit_value(40);
			const auto& result3 = dut_explicit_value.emplace(std::move(explicit_value));
			static_assert(!std::is_move_constructible_v<CopyableOnly>);
			MoveableOnly move_value(96);
			const auto& result5 = dut_move_value.emplace(std::move(move_value));
			static_assert(!std::is_move_constructible_v<NonCopyableNonMovable>);
			static_assert(!std::is_move_constructible_v<CopyConstructibleOnly>);
			MoveConstructibleOnly no_move_assign_value(69);
			const auto& result8 = dut_no_move_assign_value.emplace(std::move(no_move_assign_value));

			THEN("the Maybes contain the value")
			{
				CHECK(dut_primitive_value.has_value());
				dut_primitive_value.match([](int val) { CHECK(val == 41); }, [](std::nullopt_t) { CHECK(false); });
				CHECK(result1 == 41);

				CHECK(dut_default_value.has_value());
				dut_default_value.match(
					[](const DefaultConstructible& wrap) {
						CHECK(wrap.m_value == 42);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result2.m_value == 42);

				CHECK(dut_explicit_value.has_value());
				dut_explicit_value.match(
					[](const ExplicitConstructible& wrap) {
						CHECK(wrap.m_value == 40);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result3.m_value == 40);

				CHECK(dut_move_value.has_value());
				dut_move_value.match(
					[](const MoveableOnly& wrap) {
						CHECK(wrap.m_value == 96);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result5.m_value == 96);

				CHECK(dut_no_move_assign_value.has_value());
				dut_no_move_assign_value.match(
					[](const MoveConstructibleOnly& wrap) {
						CHECK(wrap.m_value == 69);
						CHECK(wrap.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
				CHECK(result8.m_value == 69);
			}
		}
	}

	WHEN("converting copy/move assignment")
	{
		static_assert(std::is_assignable_v<Maybe<int>&, Maybe<int>>);
		static_assert(std::is_assignable_v<Maybe<int>&, const Maybe<int>&>);
		static_assert(std::is_assignable_v<Maybe<int>&, Maybe<int>&&>);
		static_assert(std::is_assignable_v<Maybe<int>&, const Maybe<int>&&>);
		static_assert(!std::is_assignable_v<Maybe<long>&, const Maybe<int>&>);
		static_assert(!std::is_assignable_v<Maybe<long>&, Maybe<int>>);
		static_assert(!std::is_assignable_v<Maybe<long>&, Maybe<int>&&>);
		static_assert(!std::is_assignable_v<Maybe<long>&, const Maybe<int>&&>);
	}

	WHEN("emplace with initializer list")
	{
		Maybe<NonMovableList> dut;
		const auto& result = dut.emplace({1, 2, 3});

		THEN("the Maybe holds the value")
		{
			CHECK(dut.has_value());
			dut.match([](const NonMovableList& wrap) { CHECK(wrap.m_size == 3); },
					  [](std::nullopt_t) { CHECK(false); });
			CHECK(result.m_size == 3);
		}
	}
}

SCENARIO("Maybe: swap")
{
	GIVEN("two empty Maybes")
	{
		Maybe<int> dut_empty1;
		Maybe<int> dut_empty2;

		WHEN("swap")
		{
			dut_empty1.swap(dut_empty2);

			THEN("both Maybes are still empty")
			{
				CHECK_FALSE(dut_empty1.has_value());
				CHECK_FALSE(dut_empty2.has_value());
			}
		}
	}

	GIVEN("an empty Maybe and an engaged Maybe")
	{
		Maybe<int> dut_empty_lhs;
		Maybe<int> dut_value_rhs(42);
		Maybe<int> dut_empty_rhs;
		Maybe<int> dut_value_lhs(42);

		WHEN("swap")
		{
			dut_empty_lhs.swap(dut_value_rhs);
			dut_value_lhs.swap(dut_empty_rhs);

			THEN("the values are swapped")
			{
				CHECK(dut_empty_lhs.has_value());
				CHECK(dut_empty_lhs.match([](int val) { return val == 42; }, [](std::nullopt_t) { return false; }));
				CHECK_FALSE(dut_value_rhs.has_value());
				CHECK(dut_value_rhs.match([](int) { return false; }, [](std::nullopt_t) { return true; }));

				CHECK_FALSE(dut_value_lhs.has_value());
				CHECK(dut_value_lhs.match([](int) { return false; }, [](std::nullopt_t) { return true; }));
				CHECK(dut_empty_rhs.has_value());
				CHECK(dut_empty_rhs.match([](int val) { return val == 42; }, [](std::nullopt_t) { return false; }));
			}
		}
	}

	GIVEN("two engaged Maybes")
	{
		Maybe<int> dut_value1(42);
		Maybe<int> dut_value2(101);

		WHEN("swap")
		{
			dut_value1.swap(dut_value2);

			THEN("the values are swapped")
			{
				CHECK(dut_value1.match([](int val) { return val == 101; }, [](std::nullopt_t) { return false; }));
				CHECK(dut_value2.match([](int val) { return val == 42; }, [](std::nullopt_t) { return false; }));
			}
		}
	}
}

SCENARIO("Maybe: modifiers")
{
	GIVEN("an empty Maybe")
	{
		Maybe<DefaultConstructible> dut_default(11);
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move(101);

		WHEN("reset")
		{
			dut_default.reset();
			dut_no_copy_no_move.reset();

			THEN("the Maybe is empty")
			{
				CHECK_FALSE(dut_default.has_value());
				CHECK_FALSE(dut_no_copy_no_move.has_value());
			}
		}
	}

	GIVEN("an engaged Maybe")
	{
		Maybe<DefaultConstructible> dut_default(11);
		Maybe<NonCopyableNonMovable> dut_no_copy_no_move(101);

		WHEN("reset")
		{
			dut_default.reset();
			dut_no_copy_no_move.reset();

			THEN("the Maybe is empty")
			{
				CHECK_FALSE(dut_default.has_value());
				CHECK_FALSE(dut_no_copy_no_move.has_value());
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
				CHECK(dut.match([](std::nullopt_t) { return true; }, [](int) { return false; }));
			}
		}

		WHEN("calling value_or")
		{
			THEN("returns the fallback value")
			{
				CHECK(dut.value_or(6) == 6);
			}
		}

		WHEN("calling value_or on a const Maybe")
		{
			const Maybe<int> const_dut;
			const Maybe<DefaultConstructible> const_dut_explicit;

			THEN("the value can be retrieved")
			{
				CHECK(const_dut.value_or(6) == 6);

				auto result = const_dut_explicit.value_or(DefaultConstructible{66});
				CHECK(result.m_value == 66);
			}
		}
	}

	GIVEN("an engaged Maybe")
	{
		Maybe<int> dut{44};

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
				CHECK(dut.match([](std::nullopt_t) { return false; }, [](int value) { return value == 44; }));
			}
		}

		WHEN("calling value_or")
		{
			THEN("the value can be retrieved")
			{
				CHECK(dut.value_or(6) == 44);
			}
		}

		WHEN("calling value_or on a const Maybe")
		{
			const Maybe<int> const_dut_primitive{44};
			const Maybe<DefaultConstructible> const_dut_explicit{DefaultConstructible{55}};

			THEN("the value can be retrieved")
			{
				CHECK(const_dut_primitive.value_or(6) == 44);

				auto result = const_dut_explicit.value_or(DefaultConstructible{66});
				CHECK(result.m_value == 55);
			}
		}
	}
}

SCENARIO("Maybe: monadic operations")
{
	GIVEN("an empty Maybe")
	{
		Maybe<int> dut;

		WHEN("and_then (lvalue)")
		{
			bool called = false;
			auto result = dut.and_then([&called](int value) {
				called = true;
				return Maybe<std::string>(std::to_string(value));
			});

			THEN("the callback is not invoked and the result is empty")
			{
				CHECK_FALSE(called);
				CHECK_FALSE(result.has_value());
				CHECK_FALSE(dut.has_value());
			}
		}

		WHEN("and_then (const lvalue)")
		{
			const Maybe<int> const_dut;
			bool called = false;
			auto result = const_dut.and_then([&called](const int& value) {
				called = true;
				return Maybe<std::string>(std::to_string(value));
			});

			THEN("the callback is not invoked and the result is empty")
			{
				CHECK_FALSE(called);
				CHECK_FALSE(result.has_value());
				CHECK_FALSE(const_dut.has_value());
			}
		}

		WHEN("and_then (rvalue)")
		{
			bool called = false;
			auto result = std::move(dut).and_then([&called](int&& value) {
				called = true;
				return Maybe<std::string>(std::to_string(value));
			});

			THEN("the callback is not invoked and the result is empty")
			{
				CHECK_FALSE(called);
				CHECK_FALSE(result.has_value());
				CHECK_FALSE(dut.has_value());
			}
		}

		WHEN("and_then (const rvalue)")
		{
			const Maybe<int> const_dut;
			bool called = false;
			auto result = std::move(const_dut).and_then([&called](const int&& value) {
				called = true;
				return Maybe<std::string>(std::to_string(value));
			});

			THEN("the callback is not invoked and the result is empty")
			{
				CHECK_FALSE(called);
				CHECK_FALSE(result.has_value());
				CHECK_FALSE(const_dut.has_value());
			}
		}

		WHEN("transform (lvalue)")
		{
			bool called = false;
			auto result = dut.transform([&called](int value) {
				called = true;
				return std::to_string(value);
			});

			THEN("the callback is not invoked and the result is empty")
			{
				CHECK_FALSE(called);
				CHECK_FALSE(result.has_value());
				CHECK_FALSE(dut.has_value());
			}
		}

		WHEN("transform (const lvalue)")
		{
			const Maybe<int> const_dut;
			bool called = false;
			auto result = const_dut.transform([&called](const int& value) {
				called = true;
				return std::to_string(value);
			});

			THEN("the callback is not invoked and the result is empty")
			{
				CHECK_FALSE(called);
				CHECK_FALSE(result.has_value());
				CHECK_FALSE(const_dut.has_value());
			}
		}

		WHEN("transform (rvalue)")
		{
			bool called = false;
			auto result = std::move(dut).transform([&called](int&& value) {
				called = true;
				return std::to_string(value);
			});

			THEN("the result is an empty Maybe")
			{
				CHECK_FALSE(called);
				CHECK_FALSE(result.has_value());
				CHECK_FALSE(dut.has_value());
			}
		}

		WHEN("transform (const rvalue)")
		{
			bool called = false;
			const Maybe<int> const_empty{};
			auto result = std::move(const_empty).transform([&called](const int&& value) {
				called = true;
				return std::to_string(value);
			});

			THEN("the result is an empty Maybe")
			{
				CHECK_FALSE(called);
				CHECK_FALSE(result.has_value());
				CHECK_FALSE(const_empty.has_value());
			}
		}

		WHEN("or_else (lvalue)")
		{
			bool fallback_called = false;
			auto fallback = [&fallback_called]() {
				fallback_called = true;
				return Maybe<int>(0);
			};

			auto result = dut.or_else(fallback);

			THEN("the result is the fallback Maybe")
			{
				CHECK(result.has_value());
				CHECK(result.match([](int value) { return value == 0; }, [](std::nullopt_t) { return false; }));
				CHECK(fallback_called);
				CHECK_FALSE(dut.has_value());
			}
		}

		WHEN("or_else (rvalue)")
		{
			bool fallback_called = false;
			auto fallback = [&fallback_called]() {
				fallback_called = true;
				return Maybe<int>(0);
			};

			auto result = std::move(dut).or_else(fallback);

			THEN("the fallback is used for an empty rvalue Maybe")
			{
				CHECK(result.has_value());
				CHECK(result.match([](int value) { return value == 0; }, [](std::nullopt_t) { return false; }));
				CHECK(fallback_called);
				CHECK_FALSE(dut.has_value());
			}
		}
	}

	GIVEN("an engaged Maybe")
	{
		Maybe<int> dut{11};

		WHEN("and_then (lvalue)")
		{
			auto int_to_maybe_string = [](int& value) {
				++value;
				return Maybe<std::string>(std::to_string(value));
			};
			auto result = dut.and_then(int_to_maybe_string);

			THEN("the result is a Maybe with the transformed value")
			{
				CHECK(result.has_value());
				CHECK(result.match([](const std::string& str) { return str == "12"; },
								   [](std::nullopt_t) { return false; }));
				CHECK(dut.has_value());
				CHECK(dut.match([](int value) { return value == 12; }, [](std::nullopt_t) { return false; }));
			}
		}

		WHEN("and_then (const lvalue)")
		{
			const Maybe<int> const_dut{11};
			auto int_to_maybe_string = [](const int& value) { return Maybe<std::string>(std::to_string(value)); };
			auto result = const_dut.and_then(int_to_maybe_string);

			THEN("the result is a Maybe with the transformed value")
			{
				CHECK(result.has_value());
				CHECK(result.match([](const std::string& str) { return str == "11"; },
								   [](std::nullopt_t) { return false; }));
				CHECK(const_dut.has_value());
				CHECK(const_dut.match([](int value) { return value == 11; }, [](std::nullopt_t) { return false; }));
			}
		}

		WHEN("and_then (rvalue)")
		{
			auto int_to_maybe_string = [](int&& value) { return Maybe<std::string>(std::to_string(value)); };
			auto result = std::move(dut).and_then(int_to_maybe_string);

			THEN("the result is a Maybe with the transformed value")
			{
				CHECK(result.has_value());
				CHECK(result.match([](const std::string& str) { return str == "11"; },
								   [](std::nullopt_t) { return false; }));
				CHECK(dut.has_value());
				CHECK(dut.match([](int value) { return value == 11; }, [](std::nullopt_t) { return false; }));
			}
		}

		WHEN("and_then (const rvalue)")
		{
			const Maybe<int> const_dut{11};
			auto int_to_maybe_string = [](const int&& value) { return Maybe<std::string>(std::to_string(value)); };
			auto result = std::move(const_dut).and_then(int_to_maybe_string);

			THEN("the result is a Maybe with the transformed value")
			{
				CHECK(result.has_value());
				CHECK(result.match([](const std::string& str) { return str == "11"; },
								   [](std::nullopt_t) { return false; }));
				CHECK(const_dut.has_value());
				CHECK(const_dut.match([](int value) { return value == 11; }, [](std::nullopt_t) { return false; }));
			}
		}

		WHEN("transform (lvalue)")
		{
			auto int_to_string = [](int& value) {
				++value;
				return std::to_string(value);
			};
			auto result = dut.transform(int_to_string);

			THEN("the result is a Maybe with the transformed value")
			{
				CHECK(result.has_value());
				CHECK(result.match([](const std::string& str) { return str == "12"; },
								   [](std::nullopt_t) { return false; }));
				CHECK(dut.has_value());
				CHECK(dut.match([](int value) { return value == 12; }, [](std::nullopt_t) { return false; }));
			}
		}

		WHEN("transform (const lvalue)")
		{
			const Maybe<int> const_dut{11};
			auto int_to_string = [](const int& value) { return std::to_string(value); };
			auto result = const_dut.transform(int_to_string);

			THEN("the result is a Maybe with the transformed value and the source is unchanged")
			{
				CHECK(result.has_value());
				CHECK(result.match([](const std::string& str) { return str == "11"; },
								   [](std::nullopt_t) { return false; }));
				CHECK(const_dut.has_value());
				CHECK(const_dut.match([](int value) { return value == 11; }, [](std::nullopt_t) { return false; }));
			}
		}

		WHEN("transform (rvalue)")
		{
			auto int_to_string = [](int&& value) { return std::to_string(value); };
			auto result = std::move(dut).transform(int_to_string);

			THEN("the result is a Maybe with the transformed value")
			{
				CHECK(result.has_value());
				CHECK(result.match([](const std::string& str) { return str == "11"; },
								   [](std::nullopt_t) { return false; }));
				CHECK(dut.has_value());
				CHECK(dut.match([](int value) { return value == 11; }, [](std::nullopt_t) { return false; }));
			}
		}

		WHEN("transform (rvalue moves a non-trivial payload)")
		{
			Maybe<DefaultConstructible> movable{DefaultConstructible{37}};
			auto move_value = [](DefaultConstructible&& value) { return std::move(value); };
			auto result = std::move(movable).transform(move_value);

			THEN("the callback receives an rvalue and the source remains engaged")
			{
				CHECK(movable.has_value());
				movable.match([](const DefaultConstructible& value) { CHECK(value.m_method == Method::MovedFrom); },
							  [](std::nullopt_t) { CHECK(false); });
				result.match(
					[](const DefaultConstructible& value) {
						CHECK(value.m_value == 37);
						CHECK(value.m_method == Method::MoveConstructed);
					},
					[](std::nullopt_t) { CHECK(false); });
			}
		}

		WHEN("transform (const rvalue)")
		{
			const Maybe<int> const_dut{11};
			auto int_to_string = [](const int&& value) { return std::to_string(value); };
			auto result = std::move(const_dut).transform(int_to_string);

			THEN("the result is a Maybe with the transformed value")
			{
				CHECK(result.has_value());
				CHECK(result.match([](const std::string& str) { return str == "11"; },
								   [](std::nullopt_t) { return false; }));
				CHECK(const_dut.has_value());
			}
		}

		WHEN("or_else (lvalue)")
		{
			bool fallback_called = false;
			auto fallback = [&fallback_called]() {
				fallback_called = true;
				return Maybe<int>(0);
			};

			auto result = dut.or_else(fallback);

			THEN("the result is the original Maybe")
			{
				CHECK(result.has_value());
				CHECK(result.match([](int value) { return value == 11; }, [](std::nullopt_t) { return false; }));
				CHECK_FALSE(fallback_called);
				CHECK(dut.has_value());
				CHECK(dut.match([](int value) { return value == 11; }, [](std::nullopt_t) { return false; }));
			}
		}

		WHEN("or_else (rvalue)")
		{
			bool fallback_called = false;
			auto fallback = [&fallback_called]() {
				fallback_called = true;
				return Maybe<int>(0);
			};

			auto result = std::move(dut).or_else(fallback);

			THEN("the result is the original Maybe and fallback is not used")
			{
				CHECK(result.has_value());
				CHECK(result.match([](int value) { return value == 11; }, [](std::nullopt_t) { return false; }));
				CHECK_FALSE(fallback_called);
				CHECK_FALSE(dut.has_value());
			}
		}
	}
}

SCENARIO("Maybe: specialized algorithms")
{
	WHEN("swap")
	{
		Maybe<DefaultConstructible> maybe1{DefaultConstructible{1}};
		Maybe<DefaultConstructible> maybe2{DefaultConstructible{2}};

		std::swap(maybe1, maybe2);

		THEN("The values have swapped")
		{
			CHECK(maybe1.has_value());
			CHECK(maybe1.match([](const DefaultConstructible& value) { return value.m_value == 2; },
							   [](std::nullopt_t) { return false; }));

			CHECK(maybe2.has_value());
			CHECK(maybe2.match([](const DefaultConstructible& value) { return value.m_value == 1; },
							   [](std::nullopt_t) { return false; }));
		}
	}

	WHEN("make_maybe with moved object")
	{
		auto maybe = make_maybe(DefaultConstructible{1});

		THEN("the Maybe is engaged and contains the moved object")
		{
			CHECK(maybe.has_value());
			CHECK(maybe.match([](const DefaultConstructible& value) { return value.m_value == 1; },
							  [](std::nullopt_t) { return false; }));
		}
	}

	WHEN("make_maybe with in-place construction")
	{
		auto maybe = make_maybe<DefaultConstructible>(11);

		THEN("the Maybe is engaged and contains the value")
		{
			CHECK(maybe.has_value());
			CHECK(maybe.match([](const DefaultConstructible& value) { return value.m_value == 11; },
							  [](std::nullopt_t) { return false; }));
		}
	}

	WHEN("make_maybe with initializer list")
	{
		auto maybe = make_maybe<std::vector<int>>({42, 11, 1, 3});

		THEN("the Maybe is engaged and contains the value")
		{
			CHECK(maybe.has_value());
			CHECK(maybe.match([](const std::vector<int>& value) { return value.size() == 4; },
							  [](std::nullopt_t) { return false; }));
		}
	}
}
