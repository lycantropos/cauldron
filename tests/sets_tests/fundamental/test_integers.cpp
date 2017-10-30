#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/sets.h"
#include "../../factories.h"
#include "../../predicates.h"
#include "../../utils.h"
#include "../../operators.h"


template<typename T>
static void check_strategy() {
  cauldron::Requirement<std::set<T>> is_even_set(
      [&](std::set<T> set) -> bool {
        return std::all_of(set.begin(),
                           set.end(),
                           even<T>);
      });
  cauldron::Requirement<std::set<T>> is_odd_set(
      [&](std::set<T> set) -> bool {
        return std::all_of(set.begin(),
                           set.end(),
                           odd<T>);
      });

  // ``signed char`` is the smallest tested integer type
  static signed char min_integer = std::is_unsigned<T>() ?
                                   0 : std::numeric_limits<signed char>::min();
  static signed char max_integer = std::numeric_limits<signed char>::max();
  static auto numbers_range = factories::integers_range<int>(min_integer,
                                                             max_integer);

  T min_number = std::numeric_limits<T>::min();
  T max_number = std::numeric_limits<T>::max();
  auto numbers = std::make_shared<cauldron::Integers<T>>(min_number,
                                                         max_number);
  auto number_stays_in_range = in_range_checker<T>(min_number,
                                                   max_number);
  auto numbers_stay_in_range = [&](const std::set<T> &set) -> bool {
    return std::all_of(set.begin(),
                       set.end(),
                       number_stays_in_range);
  };

  SECTION("single element domain") {
    auto ones = std::make_shared<cauldron::Just<size_t>>(1);
    for (const T number: numbers_range) {
      auto single_number_set = std::set<T>{number};
      auto same_number = std::make_shared<cauldron::Just<T>>(number);
      cauldron::Sets<T> same_number_sets(ones,
                                         same_number);

      auto numbers_set = same_number_sets();

      REQUIRE(numbers_set == single_number_set);
    }
  }

  SECTION("multiple elements domain") {
    static size_t min_size = 0;
    static size_t max_size = constants::max_capacity;
    static const auto sizes =
        std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                     max_size);
    cauldron::Sets<T> numbers_sets(sizes,
                                   numbers);
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    auto set = numbers_sets();

    REQUIRE(size_stays_in_range(set.size()));
    REQUIRE(numbers_stay_in_range(set));
  }

  SECTION("filtration") {
    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    static size_t min_size = constants::min_capacity;
    static size_t max_size = sufficient_capacity(1, 2, // odd or even
                                                 cauldron::MAX_CYCLES);
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    cauldron::Sets<T> sets(sizes,
                           numbers);

    SECTION("parity") {
      auto even_sets = sets.filter(is_even_set);
      auto odd_sets = sets.filter(is_odd_set);

      auto even_set = (*even_sets)();
      auto odd_set = (*odd_sets)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(even_set.size()));
      REQUIRE(size_stays_in_range(odd_set.size()));
      REQUIRE(numbers_stay_in_range(even_set));
      REQUIRE(numbers_stay_in_range(odd_set));
      REQUIRE(is_even_set(even_set));
      REQUIRE(is_odd_set(odd_set));
    }

    SECTION("impossible") {
      auto invalid_sets = sets.filter(is_even_set)->filter(is_odd_set);

      REQUIRE_THROWS_AS((*invalid_sets)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    cauldron::Converter<std::set<T>> to_even_set(
        [&](const std::set<T> &set) -> std::set<T> {
          std::vector<T> vector;
          vector.reserve(set.size());
          std::transform(set.begin(),
                         set.end(),
                         std::back_inserter(vector),
                         to_even<T>);
          return std::set<T>(vector.begin(),
                             vector.end());
        });
    cauldron::Converter<std::set<T>> to_odd_set(
        [&](const std::set<T> &set) -> std::set<T> {
          std::vector<T> vector;
          vector.reserve(set.size());
          std::transform(set.begin(),
                         set.end(),
                         std::back_inserter(vector),
                         to_odd<T>);
          return std::set<T>(vector.begin(),
                             vector.end());
        });

    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    static size_t min_size = constants::min_capacity;
    static size_t max_size = sufficient_capacity(1, 2, // odd or even
                                                 cauldron::MAX_CYCLES);
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    cauldron::Sets<T> sets(sizes,
                           numbers);

    SECTION("parity") {
      auto even_sets = sets.map(to_even_set);
      auto odd_sets = sets.map(to_odd_set);

      auto even_set = (*even_sets)();
      auto odd_set = (*odd_sets)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(even_set.size()));
      REQUIRE(size_stays_in_range(odd_set.size()));
      REQUIRE(numbers_stay_in_range(even_set));
      REQUIRE(numbers_stay_in_range(odd_set));
      REQUIRE(is_even_set(even_set));
      REQUIRE(is_odd_set(odd_set));
    }

    SECTION("impossible") {
      auto invalid_even_sets = sets.map(to_odd_set)->filter(is_even_set);
      auto invalid_odd_sets = sets.map(to_even_set)->filter(is_odd_set);

      REQUIRE_THROWS_AS((*invalid_even_sets)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_odd_sets)(),
                        cauldron::OutOfCycles);
    }
  }
}


TEST_CASE("integers \"Sets\" strategy", "[Sets]") {
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
