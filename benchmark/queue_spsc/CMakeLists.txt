#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# SPDX-FileCopyrightText: 2025 René Moll
# SPDX-License-Identifier: MPL-2.0
#

find_package(Git REQUIRED)

function(git_submodule folder)
    execute_process(
        COMMAND
            ${GIT_EXECUTABLE} submodule update --init --recursive -- "${folder}"
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_LIST_DIR}
    )

    add_subdirectory(${folder})
endfunction()

git_submodule(boost)

add_executable(queue_spsc
    main.cpp
)

target_link_libraries(queue_spsc
    PRIVATE
        benchmark::benchmark
        Boost::lockfree
)
