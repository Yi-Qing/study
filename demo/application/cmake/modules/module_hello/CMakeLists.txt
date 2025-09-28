cmake_minimum_required(VERSION 3.10)

aux_source_directory(src/ HELLO_C_SRCS)

add_library(hello STATIC ${HELLO_C_SRCS})

target_include_directories(hello
    PRIVATE
    ${PROJECT_SOURCE_DIR}/modules/module_hello/inc/
    PUBLIC
    ${PROJECT_SOURCE_DIR}/inc/module_hello/
    )

set_target_properties(hello PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/module_hello)

if(HELLO_MODULE_TEST)
    aux_source_directory(test/ HELLO_TEST_SRCS)

    add_executable(hello_test ${HELLO_TEST_SRCS})

    target_link_libraries(hello_test PRIVATE
        hello
        )

    target_include_directories(hello_test
        PRIVATE
        ${PROJECT_SOURCE_DIR}/modules/module_hello/inc/
        PUBLIC
        ${PROJECT_SOURCE_DIR}/inc/module_hello/
        )

    set_target_properties(hello_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/module_hello)
endif()
