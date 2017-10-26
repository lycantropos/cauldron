#include <catch.hpp>
#include <iostream>
#include "../cauldron/floats.h"
#include "predicates.h"
#include "operators.h"


template<typename T>
static void check_strategy() {
  T min_possible_value = std::numeric_limits<T>::lowest();
  T min_possible_positive_value = std::numeric_limits<T>::min();
  T max_possible_value = std::numeric_limits<T>::max();

  static std::random_device random_device;

  auto distribution = std::uniform_real_distribution<T>(
      min_possible_value,
      -min_possible_positive_value);
  T min_value = distribution(random_device);
  T max_value = max_possible_value + min_value;
  strategies::Floats<T> numbers(min_value,
                                max_value);

  SECTION("stays in range") {
    T number = numbers();
    auto stays_in_range = in_range_checker<T>(min_value,
                                              max_value);

    REQUIRE(stays_in_range(number));
  }

  SECTION("invalid min/max values") {
    REQUIRE_THROWS_AS(strategies::Floats<T>(min_value,
                                            max_possible_value),
                      std::invalid_argument);
  }

  SECTION("filtration") {
    SECTION("sign") {
      auto positive_numbers = numbers.filter(positive<T>);
      auto non_positive_numbers = numbers.filter(non_positive<T>);

      auto positive_number = (*positive_numbers)();
      auto non_positive_number = (*non_positive_numbers)();

      REQUIRE(positive(positive_number));
      REQUIRE(non_positive(non_positive_number));
    }

    SECTION("impossible") {
      auto invalid_numbers =
          numbers.filter(positive<T>)->filter(non_positive<T>);

      REQUIRE_THROWS_AS((*invalid_numbers)(),
                        strategies::OutOfCycles);
    }
  }

  SECTION("mapping") {
    SECTION("sign") {
      auto positive_numbers = numbers.map(to_positive<T>);
      auto non_positive_numbers = numbers.map(to_non_positive<T>);

      auto positive_number = (*positive_numbers)();
      auto non_positive_number = (*non_positive_numbers)();

      REQUIRE(positive(positive_number));
      REQUIRE(non_positive(non_positive_number));
    }

    SECTION("impossible") {
      auto invalid_positive_numbers =
          numbers.map(to_non_positive<T>)->filter(positive<T>);
      auto invalid_non_positive_numbers =
          numbers.map(to_positive<T>)->filter(non_positive<T>);

      REQUIRE_THROWS_AS((*invalid_non_positive_numbers)(),
                        strategies::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_positive_numbers)(),
                        strategies::OutOfCycles);
    }
  }
}


TEST_CASE("\"Floats\" strategy", "[Floats]") {
  SECTION("float") {
    check_strategy<float>();
  }

  SECTION("double") {
    check_strategy<double>();
  }

  SECTION("long double") {
    check_strategy<long double>();
  }
}
