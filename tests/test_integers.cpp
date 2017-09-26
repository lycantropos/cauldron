#include <catch.hpp>
#include <iostream>
#include "../cauldron/integers.h"


template<typename T>
void check_strategy() {
  std::random_device random_device;
  auto distribution = std::uniform_int_distribution<T>();
  std::vector<T> borders{distribution(random_device),
                         distribution(random_device)};
  auto min_value = *std::min_element(borders.begin(), borders.end());
  auto max_value = *std::max_element(borders.begin(), borders.end());
  strategies::Integers<T> integers(min_value, max_value);

  SECTION("stays in range") {
    T integer = integers();

    bool stays_in_range = min_value <= integer <= max_value;
    REQUIRE(stays_in_range);
  }

  SECTION("filtration") {
    auto even = [](T number) { return number % 2 == 0; };
    auto odd = [](T number) { return number % 2 != 0; };

    SECTION("parity") {
      auto even_integers = integers.filter(even);
      auto odd_integers = integers.filter(odd);

      auto even_integer = even_integers();
      auto odd_integer = odd_integers();

      REQUIRE(even(even_integer));
      REQUIRE(odd(odd_integer));
    }

    SECTION("impossible") {
      auto non_existent_integers = integers.filter(even).filter(odd);

      REQUIRE_THROWS_AS(non_existent_integers(), strategies::OutOfTries);
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
