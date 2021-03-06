add_custom_target(unit-tests)

find_package(GTest)

function(mithril_add_test name)
    set(executable_target ${name}_unit_test)
    add_executable(${executable_target} ${name}_test.cc)

    target_link_libraries(${executable_target} PRIVATE mithril gtest gtest_main)
    target_include_directories(${executable_target} PRIVATE ${CMAKE_SOURCE_DIR}/include)

    add_test(NAME ${name} COMMAND ${executable_target})

    add_dependencies(unit-tests ${executable_target})
endfunction()

mithril_add_test(channel_pipeline)
mithril_add_test(channel)
mithril_add_test(message)
mithril_add_test(composite_buffer)
mithril_add_test(line_based_frame_decoder)
mithril_add_test(string_decoder)