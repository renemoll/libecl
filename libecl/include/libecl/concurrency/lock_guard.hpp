/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_CONCURRENCY_LOCK_GUARD_H
#define LIBECL_CONCURRENCY_LOCK_GUARD_H

#include "libecl/concurrency/mutex.hpp"

namespace libecl::concurrency {

/*!
 * \brief Lock guard for \a Mutex,
 */
class LockGuard
{
public:
	explicit LockGuard(Mutex& mutex) noexcept
		: m_handle{mutex}
	{
		m_handle.lock();
	}

	~LockGuard() noexcept
	{
		m_handle.unlock();
	};

	LockGuard(const LockGuard&) = delete;
	LockGuard& operator=(const LockGuard&) = delete;
	LockGuard(LockGuard&&) = delete;
	LockGuard& operator=(LockGuard&&) = delete;

private:
	Mutex& m_handle;
};
}  // namespace libecl::concurrency

#endif
