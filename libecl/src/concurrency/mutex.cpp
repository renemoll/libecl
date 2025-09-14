/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/concurrency/mutex.hpp"

namespace libecl::concurrency {
/*!
 * \brief Spins until the mutex can be locked.
 * \post  The mutex is locked by the caller.
 */
void Mutex::lock() noexcept
{
	while (m_handle.test_and_set(std::memory_order_acquire)) {
		//!\todo Would be nice to avoid spinning, may We could sleep
	};
}

/*!
 * \return True when the mutex was locked successfully.
 */
bool Mutex::try_lock() noexcept
{
	return !m_handle.test_and_set(std::memory_order_acquire);
}

/*!
 * \brief Releases the mutex.
 * \pre   The mutex is locked.
 */
void Mutex::unlock() noexcept
{
	m_handle.clear(std::memory_order_release);
}
}  // namespace libecl::concurrency
