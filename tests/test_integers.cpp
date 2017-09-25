#include <catch.hpp>
#include <iostream>
#include "../cauldron/integers.h"


template<typename T>
bool strategy_stays_in_range(const strategies::Integers<T> &integers,
                             T min_value,
                             T max_value) {
  auto integer = integers();
  return min_value <= integer <= max_value;
}


template<typename T>
void validate_strategy() {
  std::random_device random_device;
  auto distribution = std::uniform_int_distribution<T>();
  std::vector<T> borders{distribution(random_device),
                         distribution(random_device)};
  auto min_value = *std::min_element(borders.begin(),
                                     borders.end());
  auto max_value = *std::max_element(borders.begin(),
                                     borders.end());
  strategies::Integers<T> integers(min_value,
                                   max_value);
  SECTION("stays in range") {
    REQUIRE(strategy_stays_in_range<T>(integers,
                                       min_value,
                                       max_value));
  }
}


TEST_CASE("\"integers\" strategy", "[integers]") {
  SECTION("unsigned char") {
    validate_strategy<unsigned char>();
  }
  SECTION("signed char") {
    validate_strategy<signed char>();
  }
  SECTION("short int") {
    validate_strategy<short>();
  }
  SECTION("unsigned short int") {
    validate_strategy<unsigned short>();
  }
  SECTION("int") {
    validate_strategy<int>();
  }
  SECTION("unsigned int") {
    validate_strategy<unsigned>();
  }
  SECTION("long int") {
    validate_strategy<long>();
  }
  SECTION("unsigned long int") {
    validate_strategy<unsigned long>();
  }
  SECTION("long long int") {
    validate_strategy<long long>();
  }
  SECTION("unsigned long long int") {
    validate_strategy<unsigned long long>();
  }
}
