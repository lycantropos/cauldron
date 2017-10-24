#include <catch.hpp>
#include <iostream>
#include "../cauldron/just.h"


template<typename T>
static void check_strategy() {
  T value{};
  strategies::Just<T> just_value(value);
  bool values_are_equal = just_value() == value;
  REQUIRE(values_are_equal);
}


TEST_CASE("\"just\" strategy", "[just]") {
  SECTION("primitives") {
    SECTION("unsigned char") {
      check_strategy<bool>();
    }

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

  SECTION("objects") {
    SECTION("string") {
      check_strategy<std::string>();
    }

    SECTION("vector") {
      check_strategy<std::vector<std::string>>();
    }
  }
}
