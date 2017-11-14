#include <catch.hpp>
#include <cauldron/floats.h>

#include "predicates.h"
#include "operators.h"
#include "ordered_pair.h"


template<typename Number>
static void check_strategy() {
  auto max_possible_number = std::numeric_limits<Number>::max();

  Number min_number;
  Number max_number;
  std::tie(min_number, max_number) = ordered_pair(
      std::numeric_limits<Number>::lowest(),
      max_possible_number
  );
  auto stays_in_range = in_range_checker<Number>(min_number,
                                                 max_number);
  cauldron::Floats<Number> numbers(min_number,
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

      auto positive_number = positive_numbers();
      auto non_positive_number = non_positive_numbers();

      REQUIRE(stays_in_range(positive_number));
      REQUIRE(stays_in_range(non_positive_number));
      REQUIRE(positive(positive_number));
      REQUIRE(non_positive(non_positive_number));
    }

    SECTION("impossible") {
      auto invalid_positive_numbers =
          numbers.map(to_non_positive).filter(positive<Number>);
      auto invalid_non_positive_numbers =
          numbers.map(to_positive).filter(non_positive<Number>);

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
