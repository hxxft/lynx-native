
cmake_minimum_required(VERSION 2.6)

project(lepus)

set(CMAKE_CXX_FLAGS
    "-std=c++11 -Wall -Wextra -Wno-unused-parameter"
)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

include_directories(${CMAKE_SOURCE_DIR}/../Core)
aux_source_directory(${CMAKE_SOURCE_DIR}/../Core/lepus SOURCE_FILES)
add_library(lepus
    ${CMAKE_SOURCE_DIR}/../Core/parser/input_stream.cc
    ${CMAKE_SOURCE_DIR}/../Core/parser/input_stream.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/token.h 
    ${CMAKE_SOURCE_DIR}/../Core/lepus/scanner.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/parser.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/syntax_tree.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/exception.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/semantic_analysis.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/code_generator.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/visitor.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/op_code.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/function.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/value.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/table.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/lepus_string.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/guard.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/vm.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/context.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/heap.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/builtin.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/vm_context.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/upvalue.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/switch.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/string_api.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/base_api.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/math_api.h
    ${CMAKE_SOURCE_DIR}/../Core/lepus/string_util.h
    ${SOURCE_FILES}
    )


add_executable(lepus_execute
    lepus/main.cpp
    )

target_link_libraries(lepus_execute
    lepus
    )

set_target_properties(lepus_execute
    PROPERTIES OUTPUT_NAME lepus
    )
