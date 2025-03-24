/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef ECL_COMMUNICATION_TRANSACTION_H
#define ECL_COMMUNICATION_TRANSACTION_H

#include <cstdint>
#include <span>

namespace ecl::communication {
struct Transaction
{
	enum class Type
	{
		Read,
		ReadWrite,
		Write,
	};

	Type m_type;
	std::span<const uint8_t> m_tx;
	std::span<uint8_t> m_rx;
};
}  // namespace ecl::communication

#endif
