
cmake_minimum_required(VERSION 2.6)

project(lepus)

set(CMAKE_CXX_FLAGS
    "-std=c++11 -Wall -Wextra -Wno-unused-parameter"
)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

include_directories(${CMAKE_SOURCE_DIR}/../lynx)
aux_source_directory(${CMAKE_SOURCE_DIR}/../lynx/lepus SOURCE_FILES)
add_library(lepus
    ${CMAKE_SOURCE_DIR}/../lynx/parser/input_stream.cpp
    ${CMAKE_SOURCE_DIR}/../lynx/parser/input_stream.h
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/token.h 
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/scanner.h
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/parser.h
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/syntax_tree.h
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/exception.h
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/semantic_analysis.h
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/code_generator.h
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/visitor.h
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/op_code.h
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/function.h
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/value.h
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/interpreter.h
    ${CMAKE_SOURCE_DIR}/../lynx/lepus/table.h
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
