cmake_minimum_required(VERSION 3.10)

aux_source_directory(src/ WORLD_C_SRCS)

add_library(world STATIC ${WORLD_C_SRCS})

target_include_directories(world
    PRIVATE
    ${PROJECT_SOURCE_DIR}/modules/module_world/inc/
    PUBLIC
    ${PROJECT_SOURCE_DIR}/inc/module_world/
    )

set_target_properties(world PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/module_world)

if(WORLD_MODULE_TEST)
    aux_source_directory(test/ WORLD_TEST_SRCS)

    add_executable(world_test ${WORLD_TEST_SRCS})

    target_link_libraries(world_test PRIVATE
        world
        )

    target_include_directories(world_test
        PRIVATE
        ${PROJECT_SOURCE_DIR}/modules/module_world/inc/
        PUBLIC
        ${PROJECT_SOURCE_DIR}/inc/module_world/
        )

    set_target_properties(world_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/module_world)
endif()
