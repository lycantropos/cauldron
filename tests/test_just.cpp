#include <catch.hpp>
#include "../cauldron/just.h"


template<typename T>
static void check_strategy() {
  T value{};
  cauldron::Just<T> same_value(value);

  SECTION("generation") {
    bool value_is_the_same = same_value() == value;

    REQUIRE(value_is_the_same);
  }
}


TEST_CASE("\"just\" strategy", "[just]") {
  SECTION("fundamental") {
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

  SECTION("compound") {
    SECTION("string") {
      check_strategy<std::string>();
    }

    SECTION("vector") {
      check_strategy<std::vector<std::string>>();
    }

    SECTION("set") {
      check_strategy<std::set<std::string>>();
    }
  }
}
