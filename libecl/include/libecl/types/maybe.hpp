/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_TYPES_MAYBE_HPP
#define LIBECL_TYPES_MAYBE_HPP

#include <cassert>
#include <functional>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

namespace libecl {
namespace details {
template <class... Ts>
struct Overload : Ts...
{
	using Ts::operator()...;

#if __cplusplus >= 202302L
	consteval void operator()(auto) const
	{
		static_assert(false, "Unsupported type");
	}
#endif
};

#if __cplusplus < 202302L
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;
#endif
}  // namespace details

template <class T>
class Maybe;

template <class T>
concept is_derived_from_maybe = requires(const T& t) { []<class U>(const Maybe<U>&) {}(t); };

template <class T, class W>
constexpr bool converts_from_any_cvref = std::disjunction_v<std::is_constructible<T, W&>,
															std::is_convertible<W&, T>,
															std::is_constructible<T, W>,
															std::is_convertible<W, T>,
															std::is_constructible<T, const W&>,
															std::is_convertible<const W&, T>,
															std::is_constructible<T, const W>,
															std::is_convertible<const W, T>>;

/*!
 * \brief Maybe type, representing an optional value of type T.
 * \tparam T The type contained by the Maybe.
 *
 * Variation of std::optional in an attempt to avoid exceptions and undefined behaviour. As such, this
 * implementation does not provide functions to directly access the optionally embedded value.
 *
 * Instead, one must use one of the following approaches:
 * - the \a value_or function to provide a default value in case the Maybe is empty;
 * - the \a match function with handlers for both cases: when a value is present and when there is none.
 *
 * \todo bring helpers/tags into the namespace?
 * \todo Custom variant?
 * \todo implement transform, and_then, or_else
 */
template <typename T>
class Maybe
{
	static_assert(!std::is_same_v<std::remove_cv_t<T>, std::nullopt_t>);
	static_assert(!std::is_same_v<std::remove_cv_t<T>, std::in_place_t>);
	static_assert(std::is_object_v<T> && std::is_destructible_v<T> && !std::is_array_v<T>);

	template <typename U>
	friend class Maybe;

public:
	using value_type = T;

	/*!
	 * \brief Default constructor.
	 */
	constexpr Maybe() noexcept = default;

	/*!
	 * \brief Explicit empty construction.
	 */
	constexpr Maybe(std::nullopt_t value) noexcept
		: m_storage{value}
	{
	}

	/*!
	 * \brief Copy constructor.
	 */
	template <typename U = T>
		requires(std::is_copy_constructible_v<U>)
	constexpr Maybe(Maybe const& rhs)
		: m_storage(rhs.m_storage)
	{
	}

	/*!
	 * \brief Move constructor.
	 */
	template <typename U = T>
		requires(std::is_move_constructible_v<U>)
	constexpr Maybe(Maybe&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
		: m_storage(std::nullopt)
	{
		m_storage.swap(rhs.m_storage);
	}

	/*!
	 * \brief In place construction.
	 */
	template <typename... Args>
		requires(std::is_constructible_v<T, Args && ...>)
	constexpr explicit Maybe(std::in_place_t, Args&&... args)
		: m_storage(std::in_place_type<T>, std::forward<Args>(args)...)
	{
	}

	/*!
	 * \brief In place construction with initialisation list.
	 */
	template <typename U, typename... Args>
		requires(std::is_constructible_v<T, std::initializer_list<U>&, Args && ...>)
	constexpr explicit Maybe(std::in_place_t, std::initializer_list<U> il, Args&&... args)
		: m_storage(T(il, std::forward<Args>(args)...))
	{
	}

	/*!
	 * \brief Explicit move from value constructor.
	 */
	template <typename U = T>
		requires(std::is_constructible_v<T, U &&> &&
				 !std::is_same_v<typename std::remove_cvref_t<U>, std::in_place_t> &&
				 !std::is_same_v<typename std::remove_cvref_t<U>, Maybe<T>> && !std::is_convertible_v<U &&, T>)
	constexpr explicit Maybe(U&& value)
		: m_storage(std::in_place_type<T>, std::forward<U>(value))
	{
	}

	/*!
	 * \brief Non-explicit converting move from value constructor.
	 */
	template <typename U = T>
		requires(std::is_constructible_v<T, U &&> &&
				 !std::is_same_v<typename std::remove_cvref_t<U>, std::in_place_t> &&
				 !std::is_same_v<typename std::remove_cvref_t<U>, Maybe<T>> && std::is_convertible_v<U &&, T>)
	constexpr Maybe(U&& value)
		: m_storage(std::in_place_type<T>, std::forward<U>(value))
	{
	}

	/*!
	 * \brief Explicit converting copy constructor.
	 */
	template <typename U>
		requires(std::is_constructible_v<T, U const&> && !converts_from_any_cvref<T, Maybe<U>> &&
				 !std::is_convertible_v<U const&, T>)
	constexpr explicit Maybe(Maybe<U> const& rhs)
		: m_storage(std::nullopt)
	{
		if (auto const* val = std::get_if<U>(&rhs.m_storage)) {
			m_storage = T(*val);
		}
	}

	/*!
	 * \brief Non-explicit converting copy constructor.
	 */
	template <typename U>
		requires(std::is_constructible_v<T, U const&> && !converts_from_any_cvref<T, Maybe<U>> &&
				 std::is_convertible_v<U const&, T>)
	constexpr Maybe(Maybe<U> const& rhs)
		: m_storage(std::nullopt)
	{
		if (auto const* val = std::get_if<U>(&rhs.m_storage)) {
			m_storage = *val;
		}
	}

	/*!
	 * \brief Explicit converting move constructor.
	 */
	template <typename U>
		requires(std::is_constructible_v<T, U const&> && !converts_from_any_cvref<T, Maybe<U>> &&
				 !std::is_convertible_v<U const&, T>)
	constexpr explicit Maybe(Maybe<U>&& rhs)
		: m_storage(std::nullopt)
	{
		if (auto const* val = std::get_if<U>(&rhs.m_storage)) {
			m_storage = T(std::get<U>(std::move(rhs.m_storage)));
			rhs.m_storage = std::nullopt;
		}
	}

	/*!
	 * \brief Non-explicit converting move constructor.
	 */
	template <typename U>
		requires(std::is_constructible_v<T, U const&> && !converts_from_any_cvref<T, Maybe<U>> &&
				 std::is_convertible_v<U const&, T>)
	constexpr Maybe(Maybe<U>&& rhs)
		: m_storage(std::nullopt)
	{
		if (auto const* val = std::get_if<U>(&rhs.m_storage)) {
			m_storage = std::get<U>(std::move(rhs.m_storage));
			rhs.m_storage = std::nullopt;
		}
	}

	/*!
	 * @brief Destructor.
	 */
	~Maybe() = default;

	/*!
	 * \brief Assign to empty state.
	 */
	constexpr Maybe<T>& operator=(std::nullopt_t) noexcept
	{
		reset();
		return *this;
	}

	/*!
	 * \brief Copy assignment.
	 */
	template <typename U = T>
		requires(std::is_copy_constructible_v<U> && std::is_copy_assignable_v<U>)
	constexpr Maybe<T>& operator=(const Maybe& rhs)
	{
		if (rhs.has_value()) {
			if (has_value()) {
				m_storage = rhs.m_storage;
			} else {
				m_storage = T(std::get<U>(rhs.m_storage));
			}
		} else {
			reset();
		}
		return *this;
	}

	/*!
	 * \brief Move assignment.
	 */
	template <typename U = T>
		requires(std::is_move_constructible_v<U> && std::is_move_assignable_v<U>)
	constexpr Maybe<T>& operator=(Maybe&& rhs) noexcept(std::is_nothrow_move_assignable_v<U> &&
														std::is_nothrow_move_constructible_v<U>)
	{
		if (rhs.has_value()) {
			if (has_value()) {
				m_storage = std::move(rhs.m_storage);
			} else {
				m_storage = T(std::get<U>(std::move(rhs.m_storage)));
			}
			rhs.reset();
		} else {
			reset();
		}
		return *this;
	}

	/*!
	 * \brief Value assignment.
	 */
	template <typename U = T>
		requires(!std::is_same_v<std::remove_cvref_t<U>, Maybe> &&
				 !std::conjunction_v<std::is_scalar<T>, std::is_same<T, std::decay_t<U>>> &&
				 std::is_constructible_v<T, U> && std::is_assignable_v<T&, U>)
	constexpr Maybe<T>& operator=(U&& rhs)
	{
		if (has_value()) {
			m_storage = std::forward<U>(rhs);
		} else {
			m_storage = T(std::forward<U>(rhs));
		}
		return *this;
	}

	/*!
	 * \brief Converting copy assignment.
	 */
	template <class U>
		requires(std::is_constructible_v<T, const U&> && std::is_assignable_v<T&, const U&> &&
				 !converts_from_any_cvref<T, Maybe<U>> && !std::is_assignable_v<T&, Maybe<U>&> &&
				 !std::is_assignable_v<T&, Maybe<U> &&> && !std::is_assignable_v<T&, const Maybe<U>&> &&
				 !std::is_assignable_v<T&, const Maybe<U> &&>)
	constexpr Maybe<T>& operator=(const Maybe<U>& rhs)
	{
		if (rhs.has_value()) {
			if (has_value()) {
				m_storage = *rhs.m_storage;
			} else {
				m_storage = T(*rhs.m_storage);
			}
		} else {
			reset();
		}
		return *this;
	}

	/*!
	 * \brief Converting move assignment.
	 */
	template <class U>
		requires(std::is_constructible_v<T, U> && std::is_assignable_v<T&, U> &&
				 !converts_from_any_cvref<T, Maybe<U>> && !std::is_assignable_v<T&, Maybe<U>&> &&
				 !std::is_assignable_v<T&, Maybe<U> &&> && !std::is_assignable_v<T&, const Maybe<U>&> &&
				 !std::is_assignable_v<T&, const Maybe<U> &&>)
	constexpr Maybe<T>& operator=(Maybe<U>&& rhs)
	{
		if (rhs.has_value()) {
			if (has_value()) {
				m_storage = std::move(*rhs.m_storage);
			} else {
				m_storage = T(std::move(*rhs.m_storage));
			}
			rhs.reset();
		} else {
			reset();
		}
		return *this;
	}

	/*!
	 * \brief In-place value construction.
	 */
	template <class... Args>
		requires(std::is_constructible_v<T, Args...>)
	constexpr T& emplace(Args&&... args)
	{
		m_storage.template emplace<T>(std::forward<Args>(args)...);
		return std::get<T>(m_storage);
	}

	/*!
	 * \brief In-place value construction with initialisation list.
	 */
	template <class U, class... Args>
		requires(std::is_constructible_v<T, std::initializer_list<U>&, Args...>)
	constexpr T& emplace(std::initializer_list<U> il, Args&&... args)
	{
		m_storage.template emplace<T>(il, std::forward<Args>(args)...);
		return std::get<T>(m_storage);
	}

	template <typename U = T>
		requires(std::is_move_constructible_v<U>)
	constexpr void swap(Maybe& rhs) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_swappable_v<T>)
	{
		if (has_value()) {
			if (rhs.has_value()) {
				std::swap(m_storage, rhs.m_storage);
			} else {
				rhs.m_storage = std::move(m_storage);
				reset();
			}
		} else if (rhs.has_value()) {
			m_storage = std::move(rhs.m_storage);
			rhs.reset();
		}
	}

	/*!
	 * \brief Returns true when the Maybe contains a value, false otherwise.
	 */
	constexpr explicit operator bool() const noexcept
	{
		return has_value();
	}

	/*!
	 * \brief Returns true when the Maybe contains a value, false otherwise.
	 */
	[[nodiscard]] constexpr bool has_value() const noexcept
	{
		return match([](T const&) { return true; }, [&](std::nullopt_t) { return false; });
	}

	/*!
	 * \brief Returns either the contained value or the provided default value.
	 * \param default_value: a fallback value in case the Maybe is empty.
	 */
	template <typename U>
		requires(std::is_copy_constructible_v<T> && std::is_convertible_v<U &&, T>)
	[[nodiscard]] constexpr value_type value_or(U&& default_value) &&
	{
		return match([](T const& value) { return value; },
					 [&](std::nullopt_t) { return T{std::forward<U>(default_value)}; });
	}

	/*!
	 * \brief Returns either the contained value or the provided default value.
	 * \param default_value: a fallback value in case the Maybe is empty.
	 */
	template <typename U>
		requires(std::is_copy_constructible_v<T> && std::is_convertible_v<U &&, T>)
	[[nodiscard]] constexpr value_type value_or(U&& default_value) const&
	{
		return match([](T const& value) { return value; },
					 [&](std::nullopt_t) { return T{std::forward<U>(default_value)}; });
	}

	/*!
	 * \brief Pattern matching type accessor.
	 */
	template <class... Matchers>
		requires(sizeof...(Matchers) >= 1)
	decltype(auto) match(Matchers&&... matchers)
	{
		return std::visit(details::Overload{std::forward<Matchers>(matchers)...}, m_storage);
	}

	template <class... Matchers>
		requires(sizeof...(Matchers) >= 1)
	decltype(auto) match(Matchers&&... matchers) const
	{
		return std::visit(details::Overload{std::forward<Matchers>(matchers)...}, m_storage);
	}

	constexpr void reset() noexcept
	{
		m_storage = std::nullopt;
	}

	template <class F>
	constexpr auto and_then(F&& func) &
	{
		using U = std::remove_cvref_t<std::invoke_result_t<F, T&>>;
		if (has_value()) {
			return std::invoke(std::forward<F>(func), std::get<T>(m_storage));
		}
		return U();
	}

	template <class F>
	constexpr auto and_then(F&& func) const&
	{
		using U = std::remove_cvref_t<std::invoke_result_t<F, const T&>>;
		if (has_value()) {
			return std::invoke(std::forward<F>(func), std::get<T>(m_storage));
		}
		return U();
	}

	template <class F>
	constexpr auto and_then(F&& func) &&
	{
		using U = std::remove_cvref_t<std::invoke_result_t<F, T>>;
		if (has_value()) {
			return std::invoke(std::forward<F>(func), std::get<T>(m_storage));
		}
		return U();
	}

	template <class F>
	constexpr auto and_then(F&& func) const&&
	{
		using U = std::remove_cvref_t<std::invoke_result_t<F, const T&&>>;
		if (has_value()) {
			return std::invoke(std::forward<F>(func), std::get<T>(m_storage));
		}
		return U();
	}

	template <class F>
	constexpr auto transform(F&& func) &
	{
		using U = std::remove_cv_t<std::invoke_result_t<F, T&>>;
		if (has_value()) {
			return Maybe<U>(std::invoke(std::forward<F>(func), std::get<T>(m_storage)));
		}
		return Maybe<U>();
	}

	template <class F>
	constexpr auto transform(F&& func) const&
	{
		using U = std::remove_cv_t<std::invoke_result_t<F, T&>>;
		if (has_value()) {
			return Maybe<U>(std::invoke(std::forward<F>(func), std::get<T>(m_storage)));
		}
		return Maybe<U>();
	}

	template <class F>
	constexpr auto transform(F&& func) &&
	{
		using U = std::remove_cv_t<std::invoke_result_t<F, T>>;
		if (has_value()) {
			return Maybe<U>(std::invoke(std::forward<F>(func), std::move(std::get<T>(m_storage))));
		}
		return Maybe<U>();
	}

	template <class F>
	constexpr auto transform(F&& func) const&&
	{
		using U = std::remove_cv_t<std::invoke_result_t<F, T>>;
		if (has_value()) {
			return Maybe<U>(std::invoke(std::forward<F>(func), std::move(std::get<T>(m_storage))));
		}
		return Maybe<U>();
	}

	template <class F>
		requires(std::is_copy_constructible_v<T> && std::is_invocable_v<F>)
	constexpr Maybe or_else(F&& func) const&
	{
		if (has_value()) {
			return *this;
		}
		return std::forward<F>(func)();
	}

	template <class F>
		requires(std::is_move_constructible_v<T> && std::is_invocable_v<F>)
	constexpr Maybe or_else(F&& func) &&
	{
		if (has_value()) {
			return std::move(*this);
		}
		return std::forward<F>(func)();
	}

private:
	std::variant<T, std::nullopt_t> m_storage = std::nullopt;
};

template <class T, class U>
[[nodiscard]] constexpr bool operator==(const Maybe<T>& lhs, const Maybe<U>& rhs)
{
	if (lhs.has_value() != rhs.has_value()) {
		return false;
	}

	if (!lhs.has_value()) {
		return true;
	}

	return lhs.m_storage == rhs.m_storage;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator!=(const Maybe<T>& lhs, const Maybe<U>& rhs)
{
	return !(lhs == rhs);
}

template <class T, class U>
[[nodiscard]] constexpr bool operator<(const Maybe<T>& lhs, const Maybe<U>& rhs)
{
	if (!rhs.has_value()) {
		return false;
	}

	if (!lhs.has_value()) {
		return true;
	}

	return lhs.m_storage < rhs.m_storage;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator>(const Maybe<T>& lhs, const Maybe<U>& rhs)
{
	return rhs < lhs;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator<=(const Maybe<T>& lhs, const Maybe<U>& rhs)
{
	return !(lhs > rhs);
}

template <class T, class U>
[[nodiscard]] constexpr bool operator>=(const Maybe<T>& lhs, const Maybe<U>& rhs)
{
	return !(lhs < rhs);
}

template <class T>
[[nodiscard]] constexpr bool operator==(const Maybe<T>& lhs, std::nullopt_t rhs) noexcept
{
	std::ignore = rhs;
	return !lhs.has_value();
}

template <class T>
[[nodiscard]] constexpr std::strong_ordering operator<=>(const Maybe<T>& lhs, std::nullopt_t rhs) noexcept
{
	std::ignore = rhs;
	return lhs.has_value() <=> false;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator==(const Maybe<T>& lhs, const U& rhs)
{
	return lhs.has_value() ? lhs.m_storage == rhs : false;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator==(const T& lhs, const Maybe<U>& rhs)
{
	return rhs == lhs;
}
template <class T, class U>
[[nodiscard]] constexpr bool operator!=(const Maybe<T>& lhs, const U& rhs)
{
	return !(lhs == rhs);
}

template <class T, class U>
[[nodiscard]] constexpr bool operator!=(const T& lhs, const Maybe<U>& rhs)
{
	return rhs != lhs;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator<(const Maybe<T>& lhs, const U& rhs)
{
	return lhs.has_value() ? lhs.m_storage < rhs : true;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator<(const T& lhs, const Maybe<U>& rhs)
{
	return rhs.has_value() ? rhs.m_storage < lhs : true;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator>(const Maybe<T>& lhs, const U& rhs)
{
	return rhs < lhs;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator>(const T& lhs, const Maybe<U>& rhs)
{
	return rhs < lhs;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator<=(const Maybe<T>& lhs, const U& rhs)
{
	return !(rhs < lhs);
}

template <class T, class U>
[[nodiscard]] constexpr bool operator<=(const T& lhs, const Maybe<U>& rhs)
{
	return !(rhs < lhs);
}

template <class T, class U>
[[nodiscard]] constexpr bool operator>=(const Maybe<T>& lhs, const U& rhs)
{
	return !(lhs < rhs);
}

template <class T, class U>
[[nodiscard]] constexpr bool operator>=(const T& lhs, const Maybe<U>& rhs)
{
	return !(lhs < rhs);
}

template <class T, class U>
	requires(!is_derived_from_maybe<U>) && std::three_way_comparable_with<T, U>
[[nodiscard]] constexpr std::compare_three_way_result_t<T, U> operator<=>(const Maybe<T>& lhs, const U& rhs)
{
	return lhs.has_value() ? lhs.m_storage <=> rhs : std::strong_ordering::less;
}

template <class T>
constexpr void swap(Maybe<T>& lhs, Maybe<T>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	requires(std::is_move_constructible_v<T> && std::is_swappable_v<T>)
{
	lhs.swap(rhs);
}

template <class T>
[[nodiscard]] constexpr Maybe<std::decay_t<T>> make_maybe(T&& value)
{
	return Maybe<std::decay_t<T>>(std::forward<T>(value));
}

template <class T, class... Args>
[[nodiscard]] constexpr Maybe<T> make_maybe(Args&&... args)
{
	return Maybe<T>(std::in_place, std::forward<Args>(args)...);
}

template <class T, class U, class... Args>
[[nodiscard]] constexpr Maybe<T> make_maybe(std::initializer_list<U> il, Args&&... args)
{
	return Maybe<T>(std::in_place, il, std::forward<Args>(args)...);
}
}  // namespace libecl

#endif
