#include <catch.hpp>
#include "../cauldron/integers.h"
#include "predicates.h"
#include "operators.h"


template<typename Number>
static void check_strategy() {
  static std::random_device random_device;

  auto distribution = std::uniform_int_distribution<Number>();
  std::vector<Number> borders{distribution(random_device),
                              distribution(random_device)};
  Number min_number = *std::min_element(borders.begin(),
                                        borders.end());
  Number max_number = *std::max_element(borders.begin(),
                                        borders.end());
  auto stays_in_range = in_range_checker<Number>(min_number,
                                                 max_number);
  cauldron::Integers<Number> numbers(min_number,
                                     max_number);

  SECTION("stays in range") {
    Number number = numbers();

    REQUIRE(stays_in_range(number));
  }

  SECTION("union") {
    auto still_numbers = numbers || numbers;

    Number number = still_numbers();

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
    SECTION("parity") {
      auto even_numbers = numbers.map(to_even<Number>);
      auto odd_numbers = numbers.map(to_odd<Number>);

      auto even_number = (*even_numbers)();
      auto odd_number = (*odd_numbers)();

      REQUIRE(even(even_number));
      REQUIRE(odd(odd_number));
    }

    SECTION("impossible") {
      auto invalid_even_numbers =
          numbers.map(to_odd<Number>)->filter(even<Number>);
      auto invalid_odd_numbers =
          numbers.map(to_even<Number>)->filter(odd<Number>);

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
