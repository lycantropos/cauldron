#!/usr/bin/env bash

set -ex

conan install ..
cmake -G "CodeBlocks - Unix Makefiles" .
cmake --build . --target all -- -j 2

./bin/main
