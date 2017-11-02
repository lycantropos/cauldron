#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/builder.h"
#include "../../factories.h"
#include "../../predicates.h"
#include "../../operators.h"
#include "../../ordered_pair.h"
#include "../wrapper.h"


template<typename Number>
static void check_strategy() {
  using IntegerWrapper = Wrapper<Number>;

  cauldron::Requirement<IntegerWrapper> is_even_wrapper(
      [&](IntegerWrapper wrapper) -> bool {
        return even<Number>(wrapper.field());
      });
  cauldron::Requirement<IntegerWrapper> is_odd_wrapper(
      [&](IntegerWrapper wrapper) -> bool {
        return odd<Number>(wrapper.field());
      });

  // ``signed char`` is the smallest tested integer type
  static signed char min_integer = std::is_unsigned<Number>() ?
                                   0 : std::numeric_limits<signed char>::min();
  static signed char max_integer = std::numeric_limits<signed char>::max();
  static auto numbers_range = factories::integers_range<int>(min_integer,
                                                             max_integer);

  Number min_number;
  Number max_number;
  std::tie(min_number, max_number) = ordered_pair(
      std::numeric_limits<Number>::lowest(),
      std::numeric_limits<Number>::max()
  );
  auto number_stays_in_range = in_range_checker<Number>(min_number,
                                                        max_number);
  auto numbers = std::make_shared<cauldron::Integers<Number>>(min_number,
                                                              max_number);

  cauldron::Builder<IntegerWrapper, Number> numbers_wrappers(numbers);

  SECTION("single element domain") {
    auto ones = std::make_shared<cauldron::Just<size_t>>(1);
    for (Number number: numbers_range) {
      auto same_number = std::make_shared<cauldron::Just<Number>>(number);
      cauldron::Builder<IntegerWrapper, Number> number_wrappers(same_number);

      auto wrapper = number_wrappers();

      REQUIRE(wrapper == IntegerWrapper(number));
    }
  }

  SECTION("multiple elements domain") {
    auto wrapper = numbers_wrappers();

    REQUIRE(number_stays_in_range(wrapper.field()));
  }

  SECTION("union") {
    SECTION("single element domain") {
      auto ones = std::make_shared<cauldron::Just<size_t>>(1);
      for (Number number: numbers_range) {
        auto same_number = std::make_shared<cauldron::Just<Number>>(number);
        cauldron::Builder<IntegerWrapper, Number> number_wrappers(same_number);
        auto still_same_number_wrappers = number_wrappers || number_wrappers;

        auto same_number_wrapper = still_same_number_wrappers();

        REQUIRE(same_number_wrapper == IntegerWrapper(number));
      }
    }

    SECTION("multiple elements domain") {
      auto still_numbers_wrappers = numbers_wrappers || numbers_wrappers;

      auto wrapper = still_numbers_wrappers();

      REQUIRE(number_stays_in_range(wrapper.field()));
    }
  }

  SECTION("filtration") {
    SECTION("parity") {
      auto even_wrappers = numbers_wrappers.filter(is_even_wrapper);
      auto odd_wrappers = numbers_wrappers.filter(is_odd_wrapper);

      auto even_wrapper = (*even_wrappers)();
      auto odd_wrapper = (*odd_wrappers)();

      REQUIRE(number_stays_in_range(even_wrapper.field()));
      REQUIRE(number_stays_in_range(odd_wrapper.field()));
      REQUIRE(is_even_wrapper(even_wrapper));
      REQUIRE(is_odd_wrapper(odd_wrapper));
    }

    SECTION("impossible") {
      auto invalid_wrappers =
          numbers_wrappers.filter(is_even_wrapper)->filter(is_odd_wrapper);

      REQUIRE_THROWS_AS((*invalid_wrappers)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    cauldron::Converter<Number> to_even(to_even_operator(min_number,
                                                         max_number));
    cauldron::Converter<Number> to_odd(to_odd_operator(min_number,
                                                       max_number));
    cauldron::Converter<IntegerWrapper> to_even_wrapper(
        [&](const IntegerWrapper &wrapper) -> IntegerWrapper {
          return IntegerWrapper(to_even(wrapper.field()));
        });
    cauldron::Converter<IntegerWrapper> to_odd_wrapper(
        [&](const IntegerWrapper &wrapper) -> IntegerWrapper {
          return IntegerWrapper(to_odd(wrapper.field()));
        });

    SECTION("parity") {
      auto even_wrappers = numbers_wrappers.map(to_even_wrapper);
      auto odd_wrappers = numbers_wrappers.map(to_odd_wrapper);

      auto even_wrapper = (*even_wrappers)();
      auto odd_wrapper = (*odd_wrappers)();

      REQUIRE(number_stays_in_range(even_wrapper.field()));
      REQUIRE(number_stays_in_range(odd_wrapper.field()));
      REQUIRE(is_even_wrapper(even_wrapper));
      REQUIRE(is_odd_wrapper(odd_wrapper));
    }

    SECTION("impossible") {
      auto invalid_even_wrappers =
          numbers_wrappers.map(to_odd_wrapper)->filter(is_even_wrapper);
      auto invalid_odd_wrappers =
          numbers_wrappers.map(to_even_wrapper)->filter(is_odd_wrapper);

      REQUIRE_THROWS_AS((*invalid_even_wrappers)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_odd_wrappers)(),
                        cauldron::OutOfCycles);
    }
  }
}


TEST_CASE("integers \"Builder\" strategy", "[Builder]") {
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
