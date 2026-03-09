/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_UTILITIES_OPTIONAL_H
#define LIBECL_UTILITIES_OPTIONAL_H

#include <cassert>
#include <optional>
#include <type_traits>
#include <variant>

namespace libecl {
template <class... Ts>
struct overload : Ts...
{
	using Ts::operator()...;

	consteval void operator()(auto) const
	{
		static_assert(false, "Unsupported type");
	}
};

template <class T>
class Optional;

template <class T>
concept is_derived_from_optional = requires(const T& t) { []<class U>(const Optional<U>&) {}(t); };

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
 * \brief
 * \tparam T
 *
 * Variation of std::Optional in an attempt to avoid exceptions and undefined behaviour. As such, this
 * implementation does not provide functions to directly access the optionally embedded value. Instead, one must use
 * the match function with handlers for both cases: when a value is present and when there is none.
 *
 * \todo bring helpers/tags into the namespace?
 * \todo implement transform, and_then, or_else
 */
template <typename T>
class Optional
{
	static_assert(!std::is_same_v<std::remove_cv_t<T>, std::nullopt_t>);
	static_assert(!std::is_same_v<std::remove_cv_t<T>, std::in_place_t>);
	static_assert(std::is_object_v<T> && std::is_destructible_v<T> && !std::is_array_v<T>);

	template <typename U>
	friend class Optional;

public:
	using value_type = T;

	/*!
	 * \brief Default constructor.
	 */
	constexpr Optional() noexcept = default;

	/*!
	 * \brief Explicit empty construction.
	 */
	constexpr Optional(std::nullopt_t value) noexcept
		: m_storage{value}
	{
	}

	/*!
	 * \brief Copy constructor.
	 */
	template <typename U = T>
		requires(std::is_copy_constructible_v<U>)
	constexpr Optional(Optional const& rhs)
		: m_storage(rhs.m_storage)
	{
	}

	/*!
	 * \brief Move constructor.
	 */
	template <typename U = T>
		requires(std::is_move_constructible_v<U>)
	constexpr Optional(Optional&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
		: m_storage(std::nullopt)
	{
		m_storage.swap(rhs.m_storage);
	}

	/*!
	 * \brief In place construction.
	 */
	template <typename... Args>
		requires(std::is_constructible_v<T, Args && ...>)
	constexpr explicit Optional(std::in_place_t, Args&&... args)
		: m_storage(std::in_place_type<T>, std::forward<Args>(args)...)
	{
	}

	/*!
	 * \brief In place construction with initialisation list.
	 */
	template <typename U, typename... Args>
		requires(std::is_constructible_v<T, std::initializer_list<U>&, Args && ...>)
	constexpr explicit Optional(std::in_place_t, std::initializer_list<U> il, Args&&... args)
		: m_storage(T(il, std::forward<Args>(args)...))
	{
	}

	/*!
	 * \brief Explicit move from value constructor.
	 */
	template <typename U = T>
		requires(std::is_constructible_v<T, U &&> &&
				 !std::is_same_v<typename std::remove_cvref_t<U>, std::in_place_t> &&
				 !std::is_same_v<typename std::remove_cvref_t<U>, Optional<T>> && !std::is_convertible_v<U &&, T>)
	constexpr explicit Optional(U&& value)
		: m_storage(std::in_place_type<T>, std::forward<U>(value))
	{
	}

	/*!
	 * \brief Non-explicit converting move from value constructor.
	 */
	template <typename U = T>
		requires(std::is_constructible_v<T, U &&> &&
				 !std::is_same_v<typename std::remove_cvref_t<U>, std::in_place_t> &&
				 !std::is_same_v<typename std::remove_cvref_t<U>, Optional<T>> && std::is_convertible_v<U &&, T>)
	constexpr Optional(U&& value)
		: m_storage(std::in_place_type<T>, std::forward<U>(value))
	{
	}

	/*!
	 * \brief Explicit converting copy constructor.
	 */
	template <typename U>
		requires(std::is_constructible_v<T, U const&> && !converts_from_any_cvref<T, Optional<U>> &&
				 !std::is_convertible_v<U const&, T>)
	constexpr explicit Optional(Optional<U> const& rhs)
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
		requires(std::is_constructible_v<T, U const&> && !converts_from_any_cvref<T, Optional<U>> &&
				 std::is_convertible_v<U const&, T>)
	constexpr Optional(Optional<U> const& rhs)
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
		requires(std::is_constructible_v<T, U const&> && !converts_from_any_cvref<T, Optional<U>> &&
				 !std::is_convertible_v<U const&, T>)
	constexpr explicit Optional(Optional<U>&& rhs)
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
		requires(std::is_constructible_v<T, U const&> && !converts_from_any_cvref<T, Optional<U>> &&
				 std::is_convertible_v<U const&, T>)
	constexpr Optional(Optional<U>&& rhs)
		: m_storage(std::nullopt)
	{
		if (auto const* val = std::get_if<U>(&rhs.m_storage)) {
			m_storage = std::get<U>(std::move(rhs.m_storage));
			rhs.m_storage = std::nullopt;
		}
	}

	~Optional() = default;

	constexpr Optional<T>& operator=(std::nullopt_t) noexcept
	{
		reset();
		return *this;
	}

	template <typename U = T>
		requires(std::is_copy_constructible_v<U> && std::is_copy_assignable_v<U>)
	constexpr Optional<T>& operator=(const Optional& rhs)
	{
		if (rhs.has_value()) {
			if (has_value()) {
				m_storage = rhs.m_storage;
			} else {
				m_storage = T(rhs);
			}
		} else {
			reset();
		}
		return *this;
	}

	template <typename U = T>
		requires(std::is_move_constructible_v<U> && std::is_move_assignable_v<U>)
	constexpr Optional& operator=(Optional&& rhs) noexcept(std::is_nothrow_move_assignable_v<U> &&
														   std::is_nothrow_move_constructible_v<U>)
	{
		if (rhs.has_value()) {
			if (has_value()) {
				m_storage = std::move(rhs.m_storage);
			} else {
				assert(std::get_if<U>(&rhs.m_storage));
				m_storage = T(std::get<U>(std::move(rhs.m_storage)));
			}
		} else {
			reset();
		}
		return *this;
	}

	template <typename U = T>
		requires(!std::is_same_v<std::remove_cvref_t<U>, Optional> &&
				 !std::conjunction_v<std::is_scalar<T>, std::is_same<T, std::decay_t<U>>> &&
				 std::is_constructible_v<T, U> && std::is_assignable_v<T&, U>)
	constexpr Optional<T>& operator=(U&& v)
	{
		if (has_value()) {
			m_storage = std::forward<U>(v);
		} else {
			m_storage = T(std::forward<U>(v));
		}
		return *this;
	}

	template <class U>
		requires(std::is_constructible_v<T, const U&> && std::is_assignable_v<T&, const U&> &&
				 !converts_from_any_cvref<T, Optional<U>> && !std::is_assignable_v<T&, Optional<U>&> &&
				 !std::is_assignable_v<T&, Optional<U> &&> && !std::is_assignable_v<T&, const Optional<U>&> &&
				 !std::is_assignable_v<T&, const Optional<U> &&>)
	constexpr Optional<T>& operator=(const Optional<U>& rhs)
	{
		*this = Optional(rhs);
		return *this;
	}

	template <class U>
		requires(std::is_constructible_v<T, U> && std::is_assignable_v<T&, U> &&
				 !converts_from_any_cvref<T, Optional<U>> && !std::is_assignable_v<T&, Optional<U>&> &&
				 !std::is_assignable_v<T&, Optional<U> &&> && !std::is_assignable_v<T&, const Optional<U>&> &&
				 !std::is_assignable_v<T&, const Optional<U> &&>)
	constexpr Optional<T>& operator=(Optional<U>&& rhs)
	{
		*this = Optional(std::move(rhs));
		return *this;
	}

	template <class... Args>
		requires(std::is_constructible_v<T, Args...>)
	constexpr T& emplace(Args&&... args)
	{
		reset();
		m_storage = T(std::forward<Args>(args)...);
	}

	template <class U, class... Args>
		requires(std::is_constructible_v<T, std::initializer_list<U>&, Args...>)
	constexpr T& emplace(std::initializer_list<U> il, Args&&... args)
	{
		reset();
		m_storage = T(il, std::forward<Args>(args)...);
	}

	template <typename U = T>
		requires(std::is_move_constructible_v<U>)
	constexpr void swap(Optional& rhs) noexcept(std::is_nothrow_move_constructible_v<T> &&
												std::is_nothrow_swappable_v<T>)
	{
		if (has_value()) {
			if (rhs.has_value()) {
				swap(*this, rhs);
			} else {
				rhs.m_storage = T(std::move(m_storage));
				reset();
			}
		} else if (rhs.has_value()) {
			m_storage = T(std::move(rhs.m_storage));
			rhs.reset();
		}
	}

	/*!
	 * \brief Returns true when the Optional contains a value, false otherwise.
	 */
	constexpr explicit operator bool() const noexcept
	{
		return has_value();
	}

	/*!
	 * \brief Returns true when the Optional contains a value, false otherwise.
	 */
	[[nodiscard]] constexpr bool has_value() const noexcept
	{
		return match([](T const&) { return true; }, [&](std::nullopt_t) { return false; });
	}

	/*!
	 * \brief Returns either the contained value or the provided default value.
	 * \param default_value: a fallback value in case the Optional is empty.
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
	 * \param default_value: a fallback value in case the Optional is empty.
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
		return std::visit(overload{std::forward<Matchers>(matchers)...}, m_storage);
	}

	template <class... Matchers>
		requires(sizeof...(Matchers) >= 1)
	decltype(auto) match(Matchers&&... matchers) const
	{
		return std::visit(overload{std::forward<Matchers>(matchers)...}, m_storage);
	}

	constexpr void reset() noexcept
	{
		m_storage = std::nullopt;
	}

private:
	std::variant<T, std::nullopt_t> m_storage = std::nullopt;
};

template <class T, class U>
[[nodiscard]] constexpr bool operator==(const Optional<T>& lhs, const Optional<U>& rhs)
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
[[nodiscard]] constexpr bool operator!=(const Optional<T>& lhs, const Optional<U>& rhs)
{
	return !(lhs == rhs);
}

template <class T, class U>
[[nodiscard]] constexpr bool operator<(const Optional<T>& lhs, const Optional<U>& rhs)
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
[[nodiscard]] constexpr bool operator>(const Optional<T>& lhs, const Optional<U>& rhs)
{
	return rhs < lhs;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator<=(const Optional<T>& lhs, const Optional<U>& rhs)
{
	return !(lhs > rhs);
}

template <class T, class U>
[[nodiscard]] constexpr bool operator>=(const Optional<T>& lhs, const Optional<U>& rhs)
{
	return !(lhs < rhs);
}

template <class T>
[[nodiscard]] constexpr bool operator==(const Optional<T>& lhs, std::nullopt_t) noexcept
{
	return !lhs.has_value();
}

template <class T>
[[nodiscard]] constexpr std::strong_ordering operator<=>(const Optional<T>& lhs, std::nullopt_t) noexcept
{
	return lhs.has_value() <=> false;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator==(const Optional<T>& lhs, const U& rhs)
{
	return lhs.has_value() ? lhs.m_storage == rhs : false;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator==(const T& lhs, const Optional<U>& rhs)
{
	return rhs == lhs;
}
template <class T, class U>
[[nodiscard]] constexpr bool operator!=(const Optional<T>& lhs, const U& rhs)
{
	return !(lhs == rhs);
}

template <class T, class U>
[[nodiscard]] constexpr bool operator!=(const T& lhs, const Optional<U>& rhs)
{
	return rhs != lhs;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator<(const Optional<T>& lhs, const U& rhs)
{
	return lhs.has_value() ? lhs.m_storage < rhs : true;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator<(const T& lhs, const Optional<U>& rhs)
{
	return rhs.has_value() ? rhs.m_storage < lhs : true;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator>(const Optional<T>& lhs, const U& rhs)
{
	return rhs < lhs;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator>(const T& lhs, const Optional<U>& rhs)
{
	return rhs < lhs;
}

template <class T, class U>
[[nodiscard]] constexpr bool operator<=(const Optional<T>& lhs, const U& rhs)
{
	return !(rhs < lhs);
}

template <class T, class U>
[[nodiscard]] constexpr bool operator<=(const T& lhs, const Optional<U>& rhs)
{
	return !(rhs < lhs);
}

template <class T, class U>
[[nodiscard]] constexpr bool operator>=(const Optional<T>& lhs, const U& rhs)
{
	return !(lhs < rhs);
}

template <class T, class U>
[[nodiscard]] constexpr bool operator>=(const T& lhs, const Optional<U>& rhs)
{
	return !(lhs < rhs);
}

template <class T, class U>
	requires(!is_derived_from_optional<U>) && std::three_way_comparable_with<T, U>
[[nodiscard]] constexpr std::compare_three_way_result_t<T, U> operator<=>(const Optional<T>& lhs, const U& rhs)
{
	return lhs.has_value() ? lhs.m_storage <=> rhs : std::strong_ordering::less;
}

template <class T>
constexpr void swap(Optional<T>& lhs, Optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	requires(std::is_move_constructible_v<T> && std::is_swappable_v<T>)
{
	lhs.swap(rhs);
}

template <class T>
[[nodiscard]] constexpr Optional<std::decay_t<T>> make_optional(T&& value)
{
	return Optional<std::decay_t<T>>(std::forward<T>(value));
}

template <class T, class... Args>
[[nodiscard]] constexpr Optional<T> make_optional(Args&&... args)
{
	return Optional<T>(std::in_place, std::forward<Args>(args)...);
}

template <class T, class U, class... Args>
[[nodiscard]] constexpr Optional<T> make_optional(std::initializer_list<U> il, Args&&... args)
{
	return Optional<T>(std::in_place, il, std::forward<Args>(args)...);
}
}  // namespace libecl

#endif
