cmake_minimum_required(VERSION 3.17)
project(rune)

set(CMAKE_CXX_STANDARD 17)

find_package(Seastar REQUIRED)
find_package(Boost REQUIRED system log log_setup)

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cc)
add_library(rune STATIC ${SOURCE_FILES})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DCHECK_IS_ON)
    add_compile_definitions(DEBUG_LOGGING)

    add_compile_definitions(BOOST_LOG_DYN_LINK)

    set(Boost_USE_DEBUG_LIBS OFF)
    set(Boost_USE_RELEASE_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif ()

# TODO: Include Boost only on debug builds
target_include_directories(rune PUBLIC include PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(rune PUBLIC Seastar::seastar PRIVATE Boost::log)

add_subdirectory(examples)

enable_testing()
add_subdirectory(tests)
