/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2026 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef LIBECL_UTILITIES_CHECKSUM_H
#define LIBECL_UTILITIES_CHECKSUM_H

#include <cstdint>
#include <span>

namespace libecl::utilities {
uint16_t crc16(std::span<const std::byte> data);
uint32_t crc32(std::span<const std::byte> data);
}  // namespace libecl::utilities

#endif
