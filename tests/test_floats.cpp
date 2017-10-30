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
  auto stays_in_range = in_range_checker<T>(min_number,
                                            max_number);

  cauldron::Converter<T> to_positive(to_positive_operator(max_number));
  cauldron::Converter<T> to_non_positive(to_non_positive_operator(min_number));

  SECTION("stays in range") {
    T number = numbers();

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

      REQUIRE(stays_in_range(positive_number));
      REQUIRE(stays_in_range(non_positive_number));
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
      auto positive_numbers = numbers.map(to_positive);
      auto non_positive_numbers = numbers.map(to_non_positive);

      auto positive_number = (*positive_numbers)();
      auto non_positive_number = (*non_positive_numbers)();

      REQUIRE(stays_in_range(positive_number));
      REQUIRE(stays_in_range(non_positive_number));
      REQUIRE(positive(positive_number));
      REQUIRE(non_positive(non_positive_number));
    }

    SECTION("impossible") {
      auto invalid_positive_numbers =
          numbers.map(to_non_positive)->filter(positive<T>);
      auto invalid_non_positive_numbers =
          numbers.map(to_positive)->filter(non_positive<T>);

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
