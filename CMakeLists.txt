cmake_minimum_required(VERSION 3.8)
project(cauldron)

set(CMAKE_CXX_STANDARD 11)

include(build/conanbuildinfo.cmake)
conan_basic_setup()

set(HEADER_FILES
        cauldron/base.h
        cauldron/integers.h)
set(SOURCE_FILES
        tests/main.cpp
        tests/test_integers.cpp)
add_executable(main ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(main ${CONAN_LIBS})
