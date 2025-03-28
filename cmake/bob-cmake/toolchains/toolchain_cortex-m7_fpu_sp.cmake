#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

#
# Cortex-M7 specific compiler configuration
# With single precision FPU
#

set(ARM_API
	-mcpu=cortex-m7					# ARM Cortex-M7 CPU.
	-mfpu=fpv5-sp-d16				# FPU use FPv5 instructions.
	-mfloat-abi=hard				# Use FPU ABI.
	-mthumb							# Generate Thumb instructions.
	-mabi=aapcs						# Use 'ARM Architecture Procedure Calling Standard' ABI.
)

include("${CMAKE_CURRENT_LIST_DIR}/arm_cortex-m.cmake")
