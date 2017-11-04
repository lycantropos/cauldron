#include <catch.hpp>
#include <cauldron/just.h>


template<typename Value>
static void check_strategy() {
  static_assert(std::is_default_constructible<Value>(),
                "Expected type with default constructor.");

  Value default_value{};
  cauldron::Just<Value> same_value(default_value);

  Value value = same_value();

  REQUIRE(value == default_value);
}


TEST_CASE("\"Just\" strategy", "[Just]") {
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
