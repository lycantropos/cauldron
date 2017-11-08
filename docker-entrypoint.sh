#!/usr/bin/env bash

set -ex

cmake ..
make -j2
make install

./main

lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info '/usr/*' \
                            '*/tests/*' \
                            --output-file coverage.info
