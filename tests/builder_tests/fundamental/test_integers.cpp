#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/builder.h"
#include "../../factories.h"
#include "../../predicates.h"
#include "../../operators.h"
#include "../wrapper.h"


template<typename T>
static void check_integers_builder_strategy() {
  using IntegerWrapper = Wrapper<T>;

  auto is_even_wrapper = [&](IntegerWrapper wrapper) -> bool {
    return even<T>(wrapper.field());
  };
  auto is_odd_wrapper = [&](IntegerWrapper wrapper) -> bool {
    return odd<T>(wrapper.field());
  };

  T min_value = std::numeric_limits<T>::min();
  T max_value = std::numeric_limits<T>::max();
  auto integers = std::make_shared<cauldron::Integers<T>>(min_value,
                                                            max_value);
  auto integer_stays_in_range = in_range_checker<T>(min_value,
                                                    max_value);

  SECTION("single element domain") {
    // ``signed char`` is the smallest tested integer type
    static int min_integer = std::numeric_limits<signed char>::min();
    static int max_integer = std::numeric_limits<signed char>::max();
    static const auto integers_range =
        factories::integers_range<int>(min_integer,
                                       max_integer);
    auto ones = std::make_shared<cauldron::Just<size_t>>(1);
    for (T integer: integers_range) {
      auto single_integer_wrapper = IntegerWrapper(integer);
      auto same_integer = std::make_shared<cauldron::Just<T>>(integer);
      cauldron::Builder<IntegerWrapper, T> same_integer_wrappers(
          same_integer);

      auto wrapper = same_integer_wrappers();

      bool wrappers_are_equal = wrapper == single_integer_wrapper;
      REQUIRE(wrappers_are_equal);
    }
  }

  SECTION("multiple elements domain") {
    cauldron::Builder<IntegerWrapper, T> integers_wrappers(integers);
    auto wrapper = integers_wrappers();

    REQUIRE(integer_stays_in_range(wrapper.field()));
  }

  SECTION("filtration") {
    cauldron::Builder<IntegerWrapper, T> wrappers(integers);

    SECTION("parity") {
      auto even_wrappers = wrappers.filter(is_even_wrapper);
      auto odd_wrappers = wrappers.filter(is_odd_wrapper);

      auto even_wrapper = (*even_wrappers)();
      auto odd_wrapper = (*odd_wrappers)();

      REQUIRE(integer_stays_in_range(even_wrapper.field()));
      REQUIRE(integer_stays_in_range(odd_wrapper.field()));
      REQUIRE(is_even_wrapper(even_wrapper));
      REQUIRE(is_odd_wrapper(odd_wrapper));
    }

    SECTION("impossible") {
      auto invalid_wrappers =
          wrappers.filter(is_even_wrapper)->filter(is_odd_wrapper);

      REQUIRE_THROWS_AS((*invalid_wrappers)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    cauldron::Converter<IntegerWrapper> to_even_wrapper(
        [&](const IntegerWrapper &wrapper) -> IntegerWrapper {
          return IntegerWrapper(to_even<T>(wrapper.field()));
        });
    cauldron::Converter<IntegerWrapper> to_odd_wrapper(
        [&](const IntegerWrapper &wrapper) -> IntegerWrapper {
          return IntegerWrapper(to_odd<T>(wrapper.field()));
        });

    cauldron::Builder<IntegerWrapper, T> wrappers(integers);

    SECTION("parity") {
      auto even_wrappers = wrappers.map(to_even_wrapper);
      auto odd_wrappers = wrappers.map(to_odd_wrapper);

      auto even_wrapper = (*even_wrappers)();
      auto odd_wrapper = (*odd_wrappers)();

      REQUIRE(integer_stays_in_range(even_wrapper.field()));
      REQUIRE(integer_stays_in_range(odd_wrapper.field()));
      REQUIRE(is_even_wrapper(even_wrapper));
      REQUIRE(is_odd_wrapper(odd_wrapper));
    }

    SECTION("impossible") {
      auto invalid_even_wrappers =
          wrappers.map(to_odd_wrapper)->filter(is_even_wrapper);
      auto invalid_odd_wrappers =
          wrappers.map(to_even_wrapper)->filter(is_odd_wrapper);

      REQUIRE_THROWS_AS((*invalid_even_wrappers)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_odd_wrappers)(),
                        cauldron::OutOfCycles);
    }
  }
}


TEST_CASE("integers \"Builder\" strategy", "[Builder]") {
  SECTION("unsigned char") {
    check_integers_builder_strategy<unsigned char>();
  }

  SECTION("signed char") {
    check_integers_builder_strategy<signed char>();
  }

  SECTION("short int") {
    check_integers_builder_strategy<short>();
  }

  SECTION("unsigned short int") {
    check_integers_builder_strategy<unsigned short>();
  }

  SECTION("int") {
    check_integers_builder_strategy<int>();
  }

  SECTION("unsigned int") {
    check_integers_builder_strategy<unsigned>();
  }

  SECTION("long int") {
    check_integers_builder_strategy<long>();
  }

  SECTION("unsigned long int") {
    check_integers_builder_strategy<unsigned long>();
  }

  SECTION("long long int") {
    check_integers_builder_strategy<long long>();
  }

  SECTION("unsigned long long int") {
    check_integers_builder_strategy<unsigned long long>();
  }
}
