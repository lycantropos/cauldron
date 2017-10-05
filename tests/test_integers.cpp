#include <catch.hpp>
#include <iostream>
#include "../cauldron/integers.h"
#include "predicates.h"
#include "operators.h"


template<typename T>
static void check_strategy() {
  static std::random_device random_device;

  auto distribution = std::uniform_int_distribution<T>();
  std::vector<T> borders{distribution(random_device),
                         distribution(random_device)};
  T min_value = *std::min_element(borders.begin(),
                                  borders.end());
  T max_value = *std::max_element(borders.begin(),
                                  borders.end());
  strategies::Integers<T> integers(min_value,
                                   max_value);

  SECTION("stays in range") {
    T integer = integers();
    auto stays_in_range = in_range_checker<T>(min_value,
                                              max_value);

    REQUIRE(stays_in_range(integer));
  }

  SECTION("filtration") {
    SECTION("parity") {
      auto even_integers = integers.filter(even<T>);
      auto odd_integers = integers.filter(odd<T>);

      auto even_integer = (*even_integers)();
      auto odd_integer = (*odd_integers)();

      REQUIRE(even(even_integer));
      REQUIRE(odd(odd_integer));
    }

    SECTION("impossible") {
      auto invalid_integers = integers.filter(even<T>)->filter(odd<T>);

      REQUIRE_THROWS_AS((*invalid_integers)(),
                        strategies::OutOfCycles);
    }
  }

  SECTION("mapping") {
    SECTION("parity") {
      auto even_integers = integers.map(to_even<T>);
      auto odd_integers = integers.map(to_odd<T>);

      auto even_integer = (*even_integers)();
      auto odd_integer = (*odd_integers)();

      REQUIRE(even(even_integer));
      REQUIRE(odd(odd_integer));
    }

    SECTION("impossible") {
      auto invalid_even_integers = integers.map(to_odd<T>)->filter(even<T>);
      auto invalid_odd_integers = integers.map(to_even<T>)->filter(odd<T>);

      REQUIRE_THROWS_AS((*invalid_odd_integers)(),
                        strategies::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_even_integers)(),
                        strategies::OutOfCycles);
    }
  }
}


TEST_CASE("\"integers\" strategy", "[integers]") {
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
