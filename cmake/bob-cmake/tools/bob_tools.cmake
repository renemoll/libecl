#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

include(bob_clang_tidy)
include(bob_cppcheck)
include(bob_cpplint)
include(bob_include_what_you_use)
include(bob_flawfinder)

function(bob_configure_tools TARGET)
    bob_configure_clang_tidy(${TARGET})
    bob_configure_cppcheck(${TARGET})
    bob_configure_cpplint(${TARGET})
    bob_configure_iwyu(${TARGET})
endfunction()
