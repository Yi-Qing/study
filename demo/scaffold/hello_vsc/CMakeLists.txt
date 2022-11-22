cmake_minimum_required(VERSION 3.10)

project(hello_vsc VERSION 0.1.0)
set(LANGUAGE CXX)
set(LANGUAGE C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

aux_source_directory(./src srcs)

# Specify MSVC UTF-8 encoding
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE $<$<NOT:$<CONFIG:DEBUG>>:ON>)
endif(WIN32)

add_executable(hello_vsc
    ${srcs}
)

target_link_libraries(hello_vsc PRIVATE Qt::Widgets)
