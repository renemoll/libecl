/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_CONCURRENCY_MUTEX_H
#define LIBECL_CONCURRENCY_MUTEX_H

#include <atomic>

namespace libecl::concurrency {

/*!
 * \brief Simple mutex suitable for bare metal environments.
 * \todo  asserts on pre/post conditions.
 */
class Mutex
{
public:
	using native_handle_type = std::atomic_flag;

	constexpr Mutex() noexcept = default;
	~Mutex() noexcept = default;
	Mutex(const Mutex&) = delete;
	Mutex& operator=(const Mutex&) = delete;
	Mutex(Mutex&&) = delete;
	Mutex& operator=(Mutex&&) = delete;

	void lock() noexcept;
	[[nodiscard]] bool try_lock() noexcept;
	void unlock() noexcept;

	[[nodiscard]] native_handle_type& native_handle()
	{
		return m_handle;
	}

private:
	native_handle_type m_handle;
};
}  // namespace libecl::concurrency

#endif
