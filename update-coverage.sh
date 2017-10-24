#!/usr/bin/env bash

set -ex

# copying report from container
sudo docker container cp cauldron:/opt/cauldron/build/coverage.info coverage.info

# debug info
lcov --list coverage.info

# uploading report to CodeCov
bash <(curl -s https://codecov.io/bash) || echo "Codecov did not collect coverage reports"
