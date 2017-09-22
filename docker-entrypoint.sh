#!/usr/bin/env bash

set -ex

cd build && conan install .. && cd -
