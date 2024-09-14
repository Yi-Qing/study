cmake_minimum_required(VERSION 3.10)

aux_source_directory(src/ SRCS)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS} ${HEADER})

# add config header file, copy *.in to this CMakeLists out dir
configure_file(${PROJECT_SOURCE_DIR}/main/inc/buildCfg.h.in inc/buildCfg.h)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    hello
    world
    )

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE
    "${PROJECT_SOURCE_DIR}/main/inc/"
    "${PROJECT_BINARY_DIR}/main/inc/"
    PUBLIC
    "${PROJECT_SOURCE_DIR}/inc/module_hello/"
    "${PROJECT_SOURCE_DIR}/inc/module_world/"
    )

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)
