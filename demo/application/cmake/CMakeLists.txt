cmake_minimum_required(VERSION 3.10)

project(demo VERSION 0.1.2.3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_GENERATOR MATCHES "Ninja*")
    # enable color compilation information, need (gcc >= v4.9 || clang >= 3.5)
    add_compile_options("$<$<NOT:$<BOOL:${MSVC}>>:-fdiagnostics-color=always>")
elseif(CMAKE_GENERATOR MATCHES "Visual Studio *")
    # use multiple processes compiler
    add_compile_options("$<$<BOOL:${MSVC}>:/MP>")
endif()

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/source-charset:utf-8 /W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -Werror)
endif()

add_subdirectory(main main/)
add_subdirectory(modules modules/)
