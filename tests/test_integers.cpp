#include <catch.hpp>
#include <iostream>
#include "../cauldron/integers.h"


template<typename T>
bool is_strategy_valid() {
  std::random_device random_device;
  auto distribution = std::uniform_int_distribution<T>();
  std::vector<T> borders{distribution(random_device),
                         distribution(random_device)};
  auto min_value = *std::min_element(borders.begin(), borders.end());
  auto max_value = *std::max_element(borders.begin(), borders.end());
  strategies::Integers<T> integers(min_value, max_value);

  auto integer = integers();
  return min_value <= integer <= max_value;
}


TEST_CASE("\"integers\" strategy", "[integers]") {
  SECTION("unsigned char") {
    REQUIRE(is_strategy_valid<unsigned char>());
  }
  SECTION("signed char") {
    REQUIRE(is_strategy_valid<signed char>());
  }
  SECTION("short int") {
    REQUIRE(is_strategy_valid<short>());
  }
  SECTION("unsigned short int") {
    REQUIRE(is_strategy_valid<unsigned short>());
  }
  SECTION("int") {
    REQUIRE(is_strategy_valid<int>());
  }
  SECTION("unsigned int") {
    REQUIRE(is_strategy_valid<unsigned>());
  }
  SECTION("long int") {
    REQUIRE(is_strategy_valid<long>());
  }
  SECTION("unsigned long int") {
    REQUIRE(is_strategy_valid<unsigned long>());
  }
  SECTION("long long int") {
    REQUIRE(is_strategy_valid<long long>());
  }
  SECTION("unsigned long long int") {
    REQUIRE(is_strategy_valid<unsigned long long>());
  }
}
