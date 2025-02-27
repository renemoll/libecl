
option(BENCHMARK "Enable benchmarks" On)

if (BENCHMARK)
    find_package(benchmark REQUIRED)

    add_subdirectory(queue_spsc)
endif()
