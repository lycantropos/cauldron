#!/usr/bin/env bash

set -ex

### Configure git ###

# Pretend to be an user called Travis CI.
git config user.name "Travis CI"
git config user.email "travis@travis-ci.org"

### Generating documentation ###

doxygen

### Pushing documentation ###

cd docs/html
git init
git remote add origin https://github.com/${TRAVIS_REPO_SLUG}.git
git checkout -b gh-pages

git add --all
git commit -m "Deploy code docs to \"GitHub Pages\" from \"Travis CI\" build \"${TRAVIS_BUILD_NUMBER}\"" \
           -m "Commit: ${TRAVIS_COMMIT}"
git push --force https://${GITHUB_TOKEN}@github.com/${TRAVIS_REPO_SLUG}.git
