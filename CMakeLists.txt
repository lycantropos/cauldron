cmake_minimum_required(VERSION 3.9)
project(cauldron
        VERSION 0.0.0
        DESCRIPTION "Property-based testing inspired by hypothesis")

option(TESTS "enables building tests script" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

string(CONCAT
        CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} "
        # for WARNING
        "-Wall "
        # for warnings between C and C++
        "-pedantic")

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR})

set(HEADER_FILES
        cauldron/sieve.h
        cauldron/facility.h
        cauldron/bases.h
        cauldron/just.h
        cauldron/booleans.h
        cauldron/integers.h
        cauldron/floats.h
        cauldron/characters.h
        cauldron/strings.h
        cauldron/vectors.h
        cauldron/sets.h
        cauldron/builder.h)

set(SOURCE_FILES
        cauldron/booleans.cpp
        cauldron/characters.cpp
        cauldron/strings.cpp)

add_library(cauldron STATIC ${SOURCE_FILES})

set_target_properties(cauldron PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS cauldron
        DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

install(FILES ${HEADER_FILES}
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cauldron")

if (TESTS)
    include(CodeCoverage)

    string(CONCAT
            CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} "
            # for code coverage
            "-g -O0 -fprofile-arcs -ftest-coverage")

    set(TESTS_HEADER_FILES
            tests/globals.h
            tests/factories.h
            tests/operators.h
            tests/predicates.h
            tests/statistics.h
            tests/ordered_pair.h
            tests/utils.h
            tests/builder_tests/wrapper.h)

    set(TESTS_SOURCE_FILES
            tests/globals.cpp
            tests/factories.cpp
            tests/operators.cpp
            tests/predicates.cpp
            tests/utils.cpp
            tests/main.cpp
            tests/test_just.cpp
            tests/test_booleans.cpp
            tests/test_integers.cpp
            tests/test_floats.cpp
            tests/test_characters.cpp
            tests/test_strings.cpp
            tests/vectors_tests/fundamental/test_booleans.cpp
            tests/vectors_tests/fundamental/test_integers.cpp
            tests/vectors_tests/fundamental/test_floats.cpp
            tests/vectors_tests/fundamental/test_characters.cpp
            tests/vectors_tests/compound/test_strings.cpp
            tests/sets_tests/fundamental/test_booleans.cpp
            tests/sets_tests/fundamental/test_integers.cpp
            tests/sets_tests/fundamental/test_floats.cpp
            tests/sets_tests/fundamental/test_characters.cpp
            tests/sets_tests/compound/test_strings.cpp
            tests/builder_tests/fundamental/test_booleans.cpp
            tests/builder_tests/fundamental/test_integers.cpp
            tests/builder_tests/fundamental/test_floats.cpp
            tests/builder_tests/fundamental/test_characters.cpp
            tests/builder_tests/compound/test_strings.cpp
            tests/union_tests/fundamental/test_booleans.cpp
            tests/union_tests/fundamental/test_integers.cpp
            tests/union_tests/fundamental/test_floats.cpp
            tests/union_tests/fundamental/test_characters.cpp
            tests/union_tests/compound/test_strings.cpp)

    add_executable(main
            ${HEADER_FILES} ${SOURCE_FILES}
            ${TESTS_HEADER_FILES} ${TESTS_SOURCE_FILES})

    setup_target_for_coverage(${PROJECT_NAME}_coverage main coverage)
endif ()
