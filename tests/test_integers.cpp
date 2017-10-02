#include <catch.hpp>
#include <iostream>
#include "../cauldron/integers.h"
#include "utils.h"


template<typename T>
void check_integers_strategy() {
  std::random_device random_device;
  auto distribution = std::uniform_int_distribution<T>();
  std::vector<T> borders{distribution(random_device),
                         distribution(random_device)};
  auto min_value = *std::min_element(borders.begin(),
                                     borders.end());
  auto max_value = *std::max_element(borders.begin(),
                                     borders.end());
  strategies::Integers<T> integers(min_value, max_value);

  SECTION("stays in range") {
    T integer = integers();

    bool stays_in_range = min_value <= integer <= max_value;
    REQUIRE(stays_in_range);
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
      auto non_existent_integers = integers.filter(even<T>)->filter(odd<T>);

      REQUIRE_THROWS_AS((*non_existent_integers)(),
                        strategies::OutOfCycles);
    }
  }
}


TEST_CASE("\"integers\" strategy", "[integers]") {
  SECTION("unsigned char") {
    check_integers_strategy<unsigned char>();
  }
  SECTION("signed char") {
    check_integers_strategy<signed char>();
  }
  SECTION("short int") {
    check_integers_strategy<short>();
  }
  SECTION("unsigned short int") {
    check_integers_strategy<unsigned short>();
  }
  SECTION("int") {
    check_integers_strategy<int>();
  }
  SECTION("unsigned int") {
    check_integers_strategy<unsigned>();
  }
  SECTION("long int") {
    check_integers_strategy<long>();
  }
  SECTION("unsigned long int") {
    check_integers_strategy<unsigned long>();
  }
  SECTION("long long int") {
    check_integers_strategy<long long>();
  }
  SECTION("unsigned long long int") {
    check_integers_strategy<unsigned long long>();
  }
}
