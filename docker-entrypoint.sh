#!/usr/bin/env bash

set -ex

cd build && conan install .. && cd -

cmake -G "CodeBlocks - Unix Makefiles" .
cmake --build . --target all -- -j 2

./bin/main
