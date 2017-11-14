#!/usr/bin/env bash

set -ex

cmake -DTESTS=ON ..
make -j$(nproc --all)
make install

./main

lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info '/usr/*' \
                            '*/tests/*' \
                            --output-file coverage.info
