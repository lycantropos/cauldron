# cauldron

[![Travis CI](https://travis-ci.org/lycantropos/cauldron.svg?branch=master)](https://travis-ci.org/lycantropos/cauldron)
[![Codecov](https://codecov.io/gh/lycantropos/cauldron/branch/master/graph/badge.svg)](https://codecov.io/gh/lycantropos/cauldron)
[![ReadTheDocs](https://readthedocs.org/projects/cauldron-cpp/badge/?version=latest)](https://cauldron-cpp.readthedocs.io/en/latest)
[![Licence](https://img.shields.io/github/license/lycantropos/cauldron.svg)](https://github.com/lycantropos/cauldron/blob/master/LICENSE)

`cauldron` helps to write property-based test cases
and abstract from specific examples.

The core definition is *strategy*
(which is related to [strategy pattern](https://en.wikipedia.org/wiki/Strategy_pattern),
but not the same):
an object which encapsulates an algorithm for generating specific type of values.

Strategies can be modified

- by filtering out values which do not satisfy custom predicates,
- by transforming values with custom operators

and combined

- united into one,
- passed as an argument to another strategy).

## Usage

### Simple example

Let's build strategy which always generates `true` value

```c++
#include <iostream>
#include <cauldron/just.h>


int main() {
  cauldron::Just<bool> true_values(true);
  bool true_value = true_values();
  if (true_value) {
    std::cout << "Hello, World!" << std::endl;
    return 0;
  } else {
    std::cout << "Something went wrong!" << std::endl;
    return 1;
  };
}
```

### Complex example

Let's assume we're writing a program
that works with vectors of numbers
and we need to calculate *Euclidean norm* (aka *magnitude*) of a vector.
Formula is

![magnitude](https://latex.codecogs.com/gif.latex?%5Cforall%20x%20%5Cin%20%5Cmathbb%7BR%7D%5En%20%3A%20%5Cleft%5C%7Cx%5Cright%5C%7C_2%20%3D%20%5Csqrt%7B%20%5Cleft%7C%20x_1%20%5Cright%7C%5E2%20+%20%5Cldots%20+%20%5Cleft%7C%20x_n%20%5Cright%7C%5E2%20%7D)

where `n` is the vector space dimension.

Magnitude satisfies next ratios

1. Magnitude of zero vector equals to zero 

![zero vector](https://latex.codecogs.com/gif.latex?%5Cleft%5C%7C%5Cvec%7B0%7D%5Cright%5C%7C_2%20%3D%200)

2. Magnitude of non-zero vector is always positive

![non-zero vector](https://latex.codecogs.com/gif.latex?%5Cforall%20x%20%5Cin%20%5Cmathbb%7BR%7D%5En%20%3A%20x%20%5Cneq%20%5Cvec%7B0%7D%20%5CRightarrow%20%5Cleft%5C%7Cx%5Cright%5C%7C_2%20%3E%200)

3. [Triangle inequality](https://en.wikipedia.org/wiki/Triangle_inequality)

![triangle inequality](https://latex.codecogs.com/gif.latex?%5Cforall%20x%2C%20y%20%5Cin%20%5Cmathbb%7BR%7D%5En%20%3A%20%5Cleft%5C%7Cx%20+%20y%5Cright%5C%7C_2%20%5Cleq%20%5Cleft%5C%7Cx%5Cright%5C%7C_2%20+%20%5Cleft%5C%7Cy%5Cright%5C%7C_2)

4. Magnitude of vector with same coordinate
equals to product of dimension's square root and coordinate modulus

![same value vector](https://latex.codecogs.com/gif.latex?%5Cforall%20%5Calpha%20%5Cin%20%5Cmathbb%7BR%7D%2C%20%5Cforall%20x%20%5Cin%20%5Cmathbb%7BR%7D%5En%20%3A%20%28%5Cforall%20i%20%5Cin%20%5Coverline%7B1..n%7D%20%3A%20x_i%20%3D%20%5Calpha%29%20%5CRightarrow%20%5Cleft%5C%7Cx%5Cright%5C%7C_2%20%3D%20%5Csqrt%7Bn%7D%20%5Ccdot%20%5Cleft%7C%5Calpha%5Cright%7C)

Let's write test case using [`Catch`](https://github.com/catchorg/Catch2) framework
which checks if our implementation of *Euclidean norm* computation satisfies this ratios.

- Consider that numbers have floating point type (e.g. `double`).
- For checking triangle inequality we will overload vectors' `operator+`.

Our test case will look like

```c++
#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <functional>
#include <vector>

#include <catch.hpp>
#include <cauldron/just.h>
#include <cauldron/integers.h>
#include <cauldron/floats.h>
#include <cauldron/vectors.h>


double magnitude(const std::vector<double> &vector);


std::vector<double> operator+(const std::vector<double> &vector,
                              const std::vector<double> &other_vector) {
  assert(vector.size() == other_vector.size());

  std::vector<double> result;
  result.reserve(vector.size());

  std::transform(vector.begin(), vector.end(),
                 other_vector.begin(),
                 std::back_inserter(result),
                 std::plus<double>());
  return result;
}


TEST_CASE("Magnitude computation", "[magnitude]") {
  /*
   * we use `1` to make vectors non-empty,
   * `100` is just an upper bound to start with
   */
  cauldron::Integers<size_t> dimensions(1, 100);
  size_t dimension = dimensions();
  // considering all vectors have the same size
  cauldron::Just<size_t> sizes(dimension);
  // for now it generates values from `0.` to `1.`
  cauldron::Floats<double> elements;
  cauldron::Vectors<double> vectors(sizes,
                                    elements);

  SECTION("zero vector") {
    std::vector<double> zero_vector(dimension, 0);

    REQUIRE(magnitude(zero_vector) == 0.);
  }

  SECTION("non-zero vector") {
    auto is_non_zero_number = [](double number) -> bool {
      return number != 0;
    };
    cauldron::Requirement<std::vector<double>> is_non_zero_vector(
        [&](const std::vector<double> &vector) -> bool {
          return std::any_of(vector.begin(), vector.end(),
                             is_non_zero_number);
        });
    auto non_zero_vectors = vectors.filter(is_non_zero_vector);
    std::vector<double> non_zero_vector = non_zero_vectors();

    REQUIRE(magnitude(non_zero_vector) > 0.);
  }

  SECTION("triangle inequality") {
    std::vector<double> vector = vectors();
    std::vector<double> other_vector = vectors();

    REQUIRE(magnitude(vector + other_vector)
                <= magnitude(vector) + magnitude(other_vector));
  }

  SECTION("same value vector") {
    double element = elements();
    std::vector<double> same_value_vector(dimension, element);

    REQUIRE(magnitude(same_value_vector) == sqrt(dimension) * abs(element));
  }
}
```

As we can see there is only declaration of `magnitude`.
Straightforward definition would be

```c++
double magnitude(const std::vector<double> &vector) {
  double result = 0.;
  for (const double coordinate: vector) {
    result += pow(coordinate, 2);
  }
  return sqrt(result);
}
```

But if we change `elements` strategy to

```c++
cauldron::Just<double> elements(std::numeric_limits<double>::max()
                                    / sqrt(dimension));
```

last ratio will not be satisfied
since each coordinate squared will be greater than max possible `double` value.

If we rewrite magnitude formula like

![magnitude](https://latex.codecogs.com/gif.latex?%5Cleft%5C%7Cx%5Cright%5C%7C_2%20%3D%20%5Csqrt%7B%20%5Cleft%7C%20x_1%20%5Cright%7C%5E2%20+%20%5Cldots%20+%20%5Cleft%7C%20x_n%20%5Cright%7C%5E2%20%7D%20%3D%20%5Calpha%20%5Ccdot%20%5Csqrt%7B%20%28%5Cleft%7C%20x_1%20%5Cright%7C%20/%20%5Calpha%29%5E2%20+%20%5Cldots%20+%20%28%5Cleft%7C%20x_n%20%5Cright%7C%20/%20%5Calpha%29%5E2%20%7D)

where

![alpha](https://latex.codecogs.com/gif.latex?%5Calpha%20%3D%20%5Cmathrm%7Bmax%7D%28%5Cleft%7Cx_1%5Cright%7C%2C%20%5Cldots%2C%20%5Cleft%7Cx_n%5Cright%7C%29)

there will be no overflow.

So finally we can write

```c++
double magnitude(const std::vector<double> &vector) {
  std::vector<double> coordinates_moduli;
  coordinates_moduli.reserve(vector.size());
  std::transform(vector.begin(), vector.end(),
                 std::back_inserter(coordinates_moduli),
                 [](double number) -> double { return abs(number); });
  double max_coordinate_modulus = *std::max_element(coordinates_moduli.begin(),
                                                    coordinates_moduli.end());

  if (max_coordinate_modulus == 0.) {
    return 0.;
  }

  double result = 0.;
  for (const double coordinate: vector) {
    result += pow(coordinate / max_coordinate_modulus, 2);
  }
  return max_coordinate_modulus * sqrt(result);
}
```

and make sure that all tests pass.

## Downloading

### Release

There are no releases yet.

### Developer

Requirements:

- [`git`](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git).

Download the latest version from `GitHub` repository

```bash
git clone https://github.com/lycantropos/cauldron.git
cd cauldron
```

---

Next instructions are executed from project's directory.

---

## Installation

Requirements:

- [`CMake 3.9+`](https://cmake.org/install/),
- [`Clang 3.4+`](http://releases.llvm.org/download.html) or
  [`GCC 5.0+`](https://gcc.gnu.org/install/)
  (on `Linux` systems `Clang` will require `libstdc++5` or newer).

Create **build** directory

```bash
mkdir build
cd build
```

---
If **build** directory already exists re-create it after removing

```bash
rm -r build
mkdir build
cd build
```
---

Build and install

```bash
cmake ..
make
make install
```

## Development

### Running tests

#### Plain

Requirements:

- [`curl`](https://curl.haxx.se/download.html),
- [`Catch 2.0.1+` single header](https://github.com/catchorg/Catch2/releases),
- [`CMake 3.9+`](https://cmake.org/install/),
- [`Clang 3.4+`](http://releases.llvm.org/download.html) or
  [`GCC 5.0+`](https://gcc.gnu.org/install/)
  (on `Linux` systems `Clang` will require `libstdc++5` or newer).

Download `Catch <https://github.com/catchorg/Catch2>`__ framework header
(may require `sudo`)

```bash
cd /usr/local/include
curl -LJO https://github.com/catchorg/Catch2/releases/download/v${CATCH_VERSION}/catch.hpp
cd -
```

where `CATCH_VERSION` is the `Catch` framework version (e.g. `2.0.1`).

Create **build** directory

```bash
mkdir build
cd build
```

---------

If **build** directory already exists re-create it after removing

```bash
rm -r build
mkdir build
cd build
```

---------

Build with tests

```bash
mkdir -p build
cd build
cmake -DTESTS=ON ..
make
```

Run tests

```bash
./main
```

#### `Docker`

Requirements:

- [`Docker CE 17.05.0+`](https://www.docker.com/community-edition/) or
  [`Docker EE 17.06.1+`](https://www.docker.com/enterprise-edition/),
- [`Docker Compose 1.13.0+`](https://docs.docker.com/compose/install/).

Run tests in `Docker` containers

- directly with `Docker Compose`

  ```bash
  docker-compose up
  ```

- with `Bash` script (e.g. can be used in `Git` hooks)

  ```bash
  ./run-tests.sh
  ```

## Bumping version

Requirements:

- [`Python 2.7+`](https://www.python.org/downloads/),
- [`bumpversion`](https://github.com/peritus/bumpversion#installation).

Choose which version number category to bump following [`semver specification`](http://semver.org/).

Test bumping version
```bash
bumpversion --dry-run --verbose $VERSION
```
where `$VERSION` is the target version number category name,
possible values are `patch`/`minor`/`major`.

Bump version
```bash
bumpversion --verbose $VERSION
```

---

To avoid inconsistency between branches and pull requests,
bumping version should be merged into `master` branch as separate pull request.

---
