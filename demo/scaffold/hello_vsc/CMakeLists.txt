cmake_minimum_required(VERSION 3.16)    # ubuntu 20.04

project(hello_vsc VERSION 0.1.0)
enable_language(C)
enable_language(CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_GENERATOR MATCHES "Ninja*")
    # for use this flag, need gcc >= 4.9 || clang >= 3.5
    if ((CMAKE_C_COMPILER_ID STREQUAL "GNU") OR (CMAKE_C_COMPILER_ID STREQUAL "Clang"))
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
    endif()
    if ((CMAKE_C_COMPILER_ID STREQUAL "GNU") OR (CMAKE_C_COMPILER_ID STREQUAL "Clang"))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
    endif()
elseif(CMAKE_GENERATOR MATCHES "Visual Studio *")
    # use multiple processes compiler
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/MP>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/MP>")
endif()

if(WIN32)
    # Specify MSVC UTF-8 encoding
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    # only Debug mode create console for stdout
    set(CMAKE_WIN32_EXECUTABLE $<$<NOT:$<CONFIG:DEBUG>>:ON>)
endif(WIN32)

# set default Qt version is Qt6
find_package(Qt6 COMPONENTS Widgets)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Widgets)
endif()

set(ICON_FILE ${PROJECT_SOURCE_DIR}/resources/icon.rc)

include_directories(${PROJECT_SOURCE_DIR}/widget)

aux_source_directory(${PROJECT_SOURCE_DIR}/ SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/widget SRCS)

add_executable(${CMAKE_PROJECT_NAME}
    ${SRCS}
    ${ICON_FILE}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt::Widgets)
