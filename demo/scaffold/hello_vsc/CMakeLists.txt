cmake_minimum_required(VERSION 3.16)    # ubuntu 20.04

project(hello_vsc VERSION 0.1.0)
enable_language(C)
enable_language(CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set default Qt version is Qt6
find_package(Qt6 COMPONENTS Widgets)
if (NOT Qt6_FOUND)
    # only support Qt 5.15
    find_package(Qt5 5.15 REQUIRED COMPONENTS Widgets)
endif()

if(CMAKE_GENERATOR MATCHES "Ninja*")
    # enable color compilation information, need (gcc >= v4.9 || clang >= 3.5)
    add_compile_options("$<$<NOT:$<BOOL:${MSVC}>>:-fdiagnostics-color=always>")
elseif(CMAKE_GENERATOR MATCHES "Visual Studio *")
    # use multiple processes compiler
    add_compile_options("$<$<BOOL:${MSVC}>:/MP>")
endif()

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/utf-8 /W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -Werror)
endif()

include_directories(${PROJECT_SOURCE_DIR}/widget)

aux_source_directory(${PROJECT_SOURCE_DIR}/ SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/widget SRCS)

if (NOT Qt6_FOUND)
    # Qt5 and Qt6 compatible
    set(CMAKE_AUTORCC ON)
    qt_add_resources(SRCS ${PROJECT_SOURCE_DIR}/resources/resources.qrc)
endif()

if(WIN32)
    set(ICON_FILE ${PROJECT_SOURCE_DIR}/resources/icon.rc)
endif()

add_executable(${CMAKE_PROJECT_NAME}
    ${SRCS}
    ${ICON_FILE}
)

if (Qt6_FOUND)
    qt_add_resources(${CMAKE_PROJECT_NAME} "rcfiles"
        PREFIX "/"
        BASE "${PROJECT_SOURCE_DIR}/resources"
        FILES ${PROJECT_SOURCE_DIR}/resources/icon.ico
    )
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt::Widgets)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    # MACOSX_BUNDLE ON    # not support OSX yet
)
