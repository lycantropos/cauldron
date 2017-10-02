#include <catch.hpp>
#include <iostream>
#include "../cauldron/just.h"


template<typename T>
void check_just_strategy() {
  T value;
  strategies::Just<T> just_value(value);
  bool values_are_equal = just_value() == value;
  REQUIRE(values_are_equal);
}


TEST_CASE("\"just\" strategy", "[just]") {
  SECTION("primitives") {
    SECTION("unsigned char") {
      check_just_strategy<bool>();
    }

    SECTION("unsigned char") {
      check_just_strategy<unsigned char>();
    }

    SECTION("signed char") {
      check_just_strategy<signed char>();
    }

    SECTION("short int") {
      check_just_strategy<short>();
    }

    SECTION("unsigned short int") {
      check_just_strategy<unsigned short>();
    }

    SECTION("int") {
      check_just_strategy<int>();
    }

    SECTION("unsigned int") {
      check_just_strategy<unsigned>();
    }

    SECTION("long int") {
      check_just_strategy<long>();
    }

    SECTION("unsigned long int") {
      check_just_strategy<unsigned long>();
    }

    SECTION("long long int") {
      check_just_strategy<long long>();
    }

    SECTION("unsigned long long int") {
      check_just_strategy<unsigned long long>();
    }
  }

  SECTION("objects") {
    SECTION("string") {
      check_just_strategy<std::string>();
    }
  }
}
