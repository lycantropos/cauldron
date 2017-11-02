#include <catch.hpp>
#include "../cauldron/floats.h"
#include "predicates.h"
#include "operators.h"


template<typename Number>
static void check_strategy() {
  Number min_possible_number = std::numeric_limits<Number>::lowest();
  Number min_possible_positive_number = std::numeric_limits<Number>::min();
  Number max_possible_number = std::numeric_limits<Number>::max();

  static std::random_device random_device;

  auto distribution = std::uniform_real_distribution<Number>(
      min_possible_number,
      -min_possible_positive_number);
  Number min_number = distribution(random_device);
  Number max_number = max_possible_number + min_number;
  cauldron::Floats<Number> numbers(min_number,
                                   max_number);
  auto stays_in_range = in_range_checker<Number>(min_number,
                                                 max_number);

  cauldron::Converter<Number> to_positive(
      to_positive_operator(max_number)
  );
  cauldron::Converter<Number> to_non_positive(
      to_non_positive_operator(min_number)
  );

  SECTION("stays in range") {
    Number number = numbers();

    REQUIRE(stays_in_range(number));
  }

  SECTION("invalid min/max values") {
    REQUIRE_THROWS_AS(cauldron::Floats<Number>(min_number,
                                               max_possible_number),
                      std::invalid_argument);
  }

  SECTION("filtration") {
    SECTION("sign") {
      auto positive_numbers = numbers.filter(positive<Number>);
      auto non_positive_numbers = numbers.filter(non_positive<Number>);

      auto positive_number = (*positive_numbers)();
      auto non_positive_number = (*non_positive_numbers)();

      REQUIRE(stays_in_range(positive_number));
      REQUIRE(stays_in_range(non_positive_number));
      REQUIRE(positive(positive_number));
      REQUIRE(non_positive(non_positive_number));
    }

    SECTION("impossible") {
      auto invalid_numbers =
          numbers.filter(positive<Number>)->filter(non_positive<Number>);

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
          numbers.map(to_non_positive)->filter(positive<Number>);
      auto invalid_non_positive_numbers =
          numbers.map(to_positive)->filter(non_positive<Number>);

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
