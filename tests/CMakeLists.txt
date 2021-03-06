#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -O1 -fno-omit-frame-pointer -g")

add_custom_target(unit-tests)

find_package(GTest)

function(rune_add_test name)
    set(executable_target ${name}_unit_test)
    add_executable(${executable_target} ${name}_test.cc)

    target_link_libraries(${executable_target} PRIVATE rune gtest gtest_main)
    target_include_directories(${executable_target} PRIVATE ${CMAKE_SOURCE_DIR}/include)

    add_test(NAME ${name} COMMAND ${executable_target})

    add_dependencies(unit-tests ${executable_target})
endfunction()

rune_add_test(channel_pipeline)
rune_add_test(channel)
rune_add_test(message)
rune_add_test(composite_buffer)
rune_add_test(line_based_frame_decoder)
rune_add_test(string_decoder)