cmake_minimum_required(VERSION 3.8)
project(cauldron)

set(CMAKE_CXX_STANDARD 14)

include(build/conanbuildinfo.cmake)
conan_basic_setup()

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
        tests/globals.h
        tests/factories.h
        tests/operators.h
        tests/predicates.h
        tests/utils.h)
set(SOURCE_FILES
        cauldron/characters.cpp
        cauldron/strings.cpp
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
        tests/test_vectors.cpp)
add_executable(main ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(main ${CONAN_LIBS})
