#include <catch.hpp>
#include <cauldron/integers.h>

#include "predicates.h"
#include "operators.h"
#include "ordered_pair.h"


template<typename Number>
static void check_strategy() {
  Number min_number;
  Number max_number;
  std::tie(min_number, max_number) = ordered_pair(
      std::numeric_limits<Number>::min(),
      std::numeric_limits<Number>::max()
  );
  auto stays_in_range = in_range_checker<Number>(min_number,
                                                 max_number);
  cauldron::Integers<Number> numbers(min_number,
                                     max_number);

  SECTION("stays in range") {
    Number number = numbers();

    REQUIRE(stays_in_range(number));
  }

  SECTION("filtration") {
    SECTION("parity") {
      auto even_numbers = numbers.filter(even<Number>);
      auto odd_numbers = numbers.filter(odd<Number>);

      auto even_number = (*even_numbers)();
      auto odd_number = (*odd_numbers)();

      REQUIRE(even(even_number));
      REQUIRE(odd(odd_number));
    }

    SECTION("impossible") {
      auto invalid_numbers = numbers.filter(even<Number>)->filter(odd<Number>);

      REQUIRE_THROWS_AS((*invalid_numbers)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    cauldron::Converter<Number> to_even(to_even_operator(min_number,
                                                         max_number));
    cauldron::Converter<Number> to_odd(to_odd_operator(min_number,
                                                       max_number));

    SECTION("parity") {
      auto even_numbers = numbers.map(to_even);
      auto odd_numbers = numbers.map(to_odd);

      auto even_number = even_numbers();
      auto odd_number = odd_numbers();

      REQUIRE(even(even_number));
      REQUIRE(odd(odd_number));
    }

    SECTION("impossible") {
      auto invalid_even_numbers = numbers.map(to_odd).filter(even<Number>);
      auto invalid_odd_numbers = numbers.map(to_even).filter(odd<Number>);

      REQUIRE_THROWS_AS((*invalid_odd_numbers)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_even_numbers)(),
                        cauldron::OutOfCycles);
    }
  }
}


TEST_CASE("\"Integers\" strategy", "[Integers]") {
  SECTION("unsigned char") {
    check_strategy<unsigned char>();
  }

  SECTION("signed char") {
    check_strategy<signed char>();
  }

  SECTION("short int") {
    check_strategy<short>();
  }

  SECTION("unsigned short int") {
    check_strategy<unsigned short>();
  }

  SECTION("int") {
    check_strategy<int>();
  }

  SECTION("unsigned int") {
    check_strategy<unsigned>();
  }

  SECTION("long int") {
    check_strategy<long>();
  }

  SECTION("unsigned long int") {
    check_strategy<unsigned long>();
  }

  SECTION("long long int") {
    check_strategy<long long>();
  }

  SECTION("unsigned long long int") {
    check_strategy<unsigned long long>();
  }
}
