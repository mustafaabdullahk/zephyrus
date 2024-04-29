cmake_minimum_required(VERSION 3.28)

# set the project name
project(zephyr_prometheus_library)

# detect the platform reliably
if(ZEPHYR_BASE)
    if (NOT CONFIG_PROMETHEUS)
        return()
    endif()
    SET(ZEPHYR YES)
elseif(NOT MACOSX AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(DARWIN YES)
elseif(NOT BSD AND ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    SET(FREEBSD YES)
elseif(NOT LINUX AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(LINUX YES)
endif()

if(NOT ZEPHYR)

# add -Wall flag to enable all warnings for c
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

    # include directories
    include_directories(include)

    # add subdirectories
    add_subdirectory(src)
    add_subdirectory(tests)

else()
    add_compile_definitions(__ZEPHYR__)

    add_definitions(-DZEPHYR_VERSION=${KERNEL_VERSION_STRING})

    add_library(PROMETHEUS INTERFACE)

    zephyr_include_directories(
        ${ZEPHYR_CURRENT_MODULE_DIR}/include
    )

    target_link_libraries(PROMETHEUS INTERFACE zephyr_interface)

    zephyr_library()

    set(c-sources
        pm_collector.c
        pm_counter.c
        pm_exposer.c
        pm_formatter.c
        pm_gauge.c
        pm_histogram.c
        pm_metric.c
        pm_summary.c)

    foreach(source ${c-sources})
        list(APPEND sources ${ZEPHYR_CURRENT_MODULE_DIR}/src/${source})
    endforeach()

    zephyr_library_sources(
        ${sources}
    )

    zephyr_library_link_libraries(PROMETHEUS)

    zephyr_library_compile_options(-std=gnu99)

    if (PROMETHEUS_BUILD_TESTS)
        add_subdirectory(tests)
    endif()

endif()
