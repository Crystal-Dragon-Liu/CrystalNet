file(GLOB CRYSTALNET_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cpp")
find_program(VALGRIND_BIN valgrind)
if ("${VALGRIND_BIN}" STREQUAL "VALGRIND_BIN-NOTFOUND")
    message(WARNING "NetworkLib/test couldn't find valgrind.")
else()
    message(STATUS "NetworkLib/test found valgrind at ${VALGRIND_BIN}")
endif()

set(VALGRIND_OPTIONS
        --error-exitcode=1                      # if leaks are detected, return nonzero value
        # --gen-suppressions=all  # uncomment for leak suppression syntax
        --leak-check=full                       # detailed leak information
        --soname-synonyms=somalloc=*jemalloc*   # also intercept jemalloc
        --trace-children=yes                    # trace child processes
        --track-origins=yes                     # track origin of uninitialized values
        )
set(VALGRIND_SUPPRESSIONS_FILE "${PROJECT_SOURCE_DIR}/build_support/valgrind.supp")

##########################################
# "make LYB_test"
##########################################

foreach(crystalnet_test_source ${CRYSTALNET_TEST_SOURCES})
    #create a human readable name
    get_filename_component(crystalnet_test_filename ${crystalnet_test_source} NAME)
    string(REPLACE ".cpp" "" crystalnet_test_name ${crystalnet_test_filename})
    # Add the test target separately and as part of "make check-tests".
    add_executable(${crystalnet_test_name} EXCLUDE_FROM_ALL ${crystalnet_test_source})

    target_link_libraries(${crystalnet_test_name} CrystalNet_shared gtest gmock_main)

    # Set test target properties and dependencies.
    set_target_properties(${crystalnet_test_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
        COMMAND ${crystalnet_test_name}
    )
    # Add the test under CTest.
    add_test(${crystalnet_test_name} ${VALGRIND_BIN} ${VALGRIND_OPTIONS}
    --suppressions=${VALGRIND_SUPPRESSIONS_FILE} ${CMAKE_BINARY_DIR}/test/${crystalnet_test_name}
    --gtest_color=yes --gtest_output=xml:${CMAKE_BINARY_DIR}/test/unit_${test_name}.xml)
    add_test(${crystalnet_test_name} ${CMAKE_BINARY_DIR}/test/${crystalnet_test_name} --gtest_color=yes
            --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${crystalnet_test_name}.xml)


endforeach(crystalnet_test_source)
