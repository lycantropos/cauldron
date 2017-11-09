#include <catch.hpp>
#include <cauldron/integers.h>

#include <tests/statistics.h>
#include <tests/ordered_pair.h>


template<typename Number>
static auto numbers_factory(Number min_number,
                            Number max_number) {
  static_assert(std::is_integral<Number>(),
                "``Number`` type should be integral.");

  std::tie(min_number, max_number) = ordered_pair(min_number, max_number);
  return cauldron::Integers<Number>(min_number,
                                    max_number);
}


template<typename Number>
static void check_strategy() {
  static_assert(std::is_integral<Number>(),
                "``Number`` type should be integral.");

  Number min_number;
  Number max_number;
  std::tie(min_number, max_number) = ordered_pair<Number>(
      std::numeric_limits<Number>::min(),
      std::numeric_limits<Number>::max()
  );

  auto numbers = numbers_factory<Number>(min_number,
                                         max_number);
  auto other_numbers = numbers_factory<Number>(min_number,
                                               max_number);
  auto another_numbers = numbers_factory<Number>(min_number,
                                                 max_number);

  SECTION("idempotence") {
    auto union_with_self = numbers || numbers;

    bool strategies_are_similar =
        are_generators_similar<Number, double>(numbers,
                                               union_with_self,
                                               min_number,
                                               max_number);
    REQUIRE(strategies_are_similar);
  }

  SECTION("commutativity") {
    auto numbers_union = numbers || other_numbers;
    auto reverse_numbers_union = other_numbers || numbers;

    bool unions_are_similar =
        are_generators_similar<Number, double>(numbers_union,
                                               reverse_numbers_union,
                                               min_number,
                                               max_number);
    REQUIRE(unions_are_similar);
  }

  SECTION("associativity") {
    auto left_numbers_union = (numbers || other_numbers) || another_numbers;
    auto right_numbers_union = numbers || (other_numbers || another_numbers);

    bool strategies_are_similar =
        are_generators_similar<Number, double>(left_numbers_union,
                                               right_numbers_union,
                                               min_number,
                                               max_number);
    REQUIRE(strategies_are_similar);
  }
}


TEST_CASE("integers \"Union\" strategy", "[Union]") {
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
