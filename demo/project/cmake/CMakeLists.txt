cmake_minimum_required(VERSION 3.10)

project(demo VERSION 0.1.2.3)

if(NOT QT_QMAKE_EXECUTABLE AND (CMAKE_GENERATOR MATCHES "Ninja*"))
    if (((CMAKE_C_COMPILER_ID STREQUAL "GNU") AND
        (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 4.9)) OR
        ((CMAKE_C_COMPILER_ID STREQUAL "Clang") AND
        (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 3.5)))
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
        message(STATUS "add C flag -fdiagnostics-color=always to enable color message")
    endif()
    if (((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND
        (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 4.9)) OR
        ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") AND
        (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 3.5)))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
        message(STATUS "add CXX flag -fdiagnostics-color=always to enable color message")
    endif()
endif()

add_subdirectory(main main/)
add_subdirectory(modules modules/)

