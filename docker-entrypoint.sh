#!/usr/bin/env bash

set -ex

conan install ..
cmake ..
cmake --build . --target all -- -j 2

./bin/main

lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info '/usr/*' \
                            '/opt/cauldron/tests/*' \
                            '/root/.conan/*' \
                            --output-file coverage.info
