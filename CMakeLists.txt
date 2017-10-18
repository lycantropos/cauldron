cmake_minimum_required(VERSION 3.8)
project(cauldron)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 14)

set(HEADER_FILES
        cauldron/sieve.h
        cauldron/facility.h
        cauldron/bases.h
        cauldron/just.h
        cauldron/booleans.h
        cauldron/integers.h
        cauldron/characters.h
        cauldron/strings.h
        cauldron/vectors.h
        cauldron/sets.h)
set(SOURCE_FILES
        cauldron/booleans.cpp
        cauldron/characters.cpp
        cauldron/strings.cpp)
set(TESTS_HEADER_FILES
        tests/globals.h
        tests/factories.h
        tests/operators.h
        tests/predicates.h
        tests/utils.h)
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
        tests/test_characters.cpp
        tests/test_strings.cpp
        tests/test_vectors.cpp
        tests/test_sets.cpp)
add_executable(main
        ${HEADER_FILES} ${SOURCE_FILES}
        ${TESTS_HEADER_FILES} ${TESTS_SOURCE_FILES})
target_link_libraries(main ${CONAN_LIBS})
add_library(cauldron ${HEADER_FILES} ${SOURCE_FILES})
