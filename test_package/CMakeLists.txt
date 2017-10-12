cmake_minimum_required(VERSION 3.8)
project(cauldron_test CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 14)

add_executable(example example.cpp)
target_link_libraries(example ${CONAN_LIBS})
