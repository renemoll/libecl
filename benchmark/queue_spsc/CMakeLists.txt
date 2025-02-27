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
