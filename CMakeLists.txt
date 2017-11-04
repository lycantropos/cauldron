cmake_minimum_required(VERSION 3.8)
project(cauldron)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# add the project specific cmake modules directory to the cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
# include the cmake code coverage module
include(CodeCoverage)

set(CMAKE_CXX_STANDARD 14)

SET(BASEPATH "${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${BASEPATH}")

string(CONCAT
        CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} "
        # for WARNING
        "-Wall "
        # for warnings between C and C++
        "-pedantic "
        # for code coverage
        "-g -O0 -fprofile-arcs -ftest-coverage")

# cmake code coverage module target
setup_target_for_coverage(${PROJECT_NAME}_coverage main coverage)

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

add_library(cauldron ${HEADER_FILES} ${SOURCE_FILES})

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
target_link_libraries(main ${CONAN_LIBS})

enable_testing()
add_test(NAME cauldron_tests COMMAND main)
