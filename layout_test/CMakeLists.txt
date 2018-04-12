
cmake_minimum_required(VERSION 2.6)

project(layout_test)

set(CMAKE_CXX_FLAGS
    "-std=c++11 -Wall -Wextra -Wno-unused-parameter -DTESTING"
)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

include_directories(${CMAKE_SOURCE_DIR}/../lynx)
include_directories(${CMAKE_SOURCE_DIR}/../lynx/third_party/googletest/include
                    ${CMAKE_SOURCE_DIR}/../lynx/third_party/googletest)
include_directories(${CMAKE_SOURCE_DIR}/../layout_test/cxx)

add_library(layout_test
    ${CMAKE_SOURCE_DIR}/../lynx/third_party/googletest/src/gtest-all.cc
    ${CMAKE_SOURCE_DIR}/../lynx/base/string/string_number_convert.cc
    ${CMAKE_SOURCE_DIR}/../lynx/base/string/string_utils.cc
    ${CMAKE_SOURCE_DIR}/../lynx/layout/container_node.cc
    ${CMAKE_SOURCE_DIR}/../lynx/layout/container_node.h 
    ${CMAKE_SOURCE_DIR}/../lynx/layout/css_color.cc
    ${CMAKE_SOURCE_DIR}/../lynx/layout/css_color.h
    ${CMAKE_SOURCE_DIR}/../lynx/layout/css_layout.cc
    ${CMAKE_SOURCE_DIR}/../lynx/layout/css_layout.h
    ${CMAKE_SOURCE_DIR}/../lynx/layout/css_style_config.h
    ${CMAKE_SOURCE_DIR}/../lynx/layout/css_style.cc
    ${CMAKE_SOURCE_DIR}/../lynx/layout/css_style.h
    ${CMAKE_SOURCE_DIR}/../lynx/layout/css_type.cc
    ${CMAKE_SOURCE_DIR}/../lynx/layout/css_type.h
    ${CMAKE_SOURCE_DIR}/../lynx/layout/layout_object.cc
    ${CMAKE_SOURCE_DIR}/../lynx/layout/layout_object.h
    ${CMAKE_SOURCE_DIR}/../lynx/layout/node.h
    ${CMAKE_SOURCE_DIR}/../lynx/layout/mock_layout_host.h
    )


add_executable(layout_test_execute
    src/main.cpp
    )

target_link_libraries(layout_test_execute
    layout_test
    )

set_target_properties(layout_test_execute
    PROPERTIES OUTPUT_NAME layout_test
    )
