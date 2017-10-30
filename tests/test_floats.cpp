#include <catch.hpp>
#include "../cauldron/floats.h"
#include "predicates.h"
#include "operators.h"


template<typename T>
static void check_strategy() {
  T min_possible_number = std::numeric_limits<T>::lowest();
  T min_possible_positive_number = std::numeric_limits<T>::min();
  T max_possible_number = std::numeric_limits<T>::max();

  static std::random_device random_device;

  auto distribution = std::uniform_real_distribution<T>(
      min_possible_number,
      -min_possible_positive_number);
  T min_number = distribution(random_device);
  T max_number = max_possible_number + min_number;
  cauldron::Floats<T> numbers(min_number,
                              max_number);

  SECTION("stays in range") {
    T number = numbers();
    auto stays_in_range = in_range_checker<T>(min_number,
                                              max_number);

    REQUIRE(stays_in_range(number));
  }

  SECTION("invalid min/max values") {
    REQUIRE_THROWS_AS(cauldron::Floats<T>(min_number,
                                          max_possible_number),
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
                        cauldron::OutOfCycles);
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
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_positive_numbers)(),
                        cauldron::OutOfCycles);
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
