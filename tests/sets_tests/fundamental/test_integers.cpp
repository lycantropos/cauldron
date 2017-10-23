#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/sets.h"
#include "../../factories.h"
#include "../../predicates.h"
#include "../../utils.h"
#include "../../operators.h"


template<typename T>
static void check_integers_sets_strategy() {
  strategies::Requirement<std::set<T>> is_even_set(
      [&](std::set<T> set) -> bool {
        return std::all_of(set.begin(),
                           set.end(),
                           even<T>);
      });
  strategies::Requirement<std::set<T>> is_odd_set(
      [&](std::set<T> set) -> bool {
        return std::all_of(set.begin(),
                           set.end(),
                           odd<T>);
      });

  T min_value = std::numeric_limits<T>::min();
  T max_value = std::numeric_limits<T>::max();
  auto integers = std::make_shared<strategies::Integers<T>>(min_value,
                                                            max_value);
  auto integer_stays_in_range = in_range_checker<T>(min_value,
                                                    max_value);
  auto integers_stay_in_range =
      [&](const std::set<T> &integers_set) -> bool {
        return std::all_of(integers_set.begin(),
                           integers_set.end(),
                           integer_stays_in_range);
      };

  SECTION("single element domain") {
    // ``signed char`` is the smallest tested integer type
    static int min_integer = std::numeric_limits<signed char>::min();
    static int max_integer = std::numeric_limits<signed char>::max();
    static const auto integers_range =
        factories::integers_range<int>(min_integer,
                                       max_integer);
    auto ones = std::make_shared<strategies::Just<size_t>>(1);
    for (T integer: integers_range) {
      auto single_integer_set = std::set<T>{integer};
      auto same_integer = std::make_shared<strategies::Just<T>>(integer);
      strategies::Sets<T> same_integer_sets(ones,
                                            same_integer);

      auto set = same_integer_sets();

      bool sets_are_equal = set == single_integer_set;
      REQUIRE(sets_are_equal);
    }
  }

  SECTION("multiple elements domain") {
    static size_t min_size = 0;
    static size_t max_size = constants::max_capacity;
    static const auto sizes =
        std::make_shared<strategies::Integers<size_t>>(min_size,
                                                       max_size);
    strategies::Sets<T> integers_sets(sizes,
                                      integers);
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    auto set = integers_sets();

    REQUIRE(size_stays_in_range(set.size()));
    REQUIRE(integers_stay_in_range(set));
  }

  SECTION("filtration") {
    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    static size_t min_size = constants::min_capacity;
    static size_t max_size = sufficient_capacity(1, 2, // odd or even
                                                 strategies::MAX_CYCLES);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    strategies::Sets<T> sets(sizes,
                             integers);

    SECTION("parity") {
      auto even_sets = sets.filter(is_even_set);
      auto odd_sets = sets.filter(is_odd_set);

      auto even_set = (*even_sets)();
      auto odd_set = (*odd_sets)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(even_set.size()));
      REQUIRE(size_stays_in_range(odd_set.size()));
      REQUIRE(integers_stay_in_range(even_set));
      REQUIRE(integers_stay_in_range(odd_set));
      REQUIRE(is_even_set(even_set));
      REQUIRE(is_odd_set(odd_set));
    }

    SECTION("impossible") {
      auto invalid_sets = sets.filter(is_even_set)->filter(is_odd_set);

      REQUIRE_THROWS_AS((*invalid_sets)(),
                        strategies::OutOfCycles);
    }
  }

  SECTION("mapping") {
    strategies::Converter<std::set<T>> to_even_set(
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
    strategies::Converter<std::set<T>> to_odd_set(
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
                                                 strategies::MAX_CYCLES);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    strategies::Sets<T> sets(sizes,
                             integers);

    SECTION("parity") {
      auto even_sets = sets.map(to_even_set);
      auto odd_sets = sets.map(to_odd_set);

      auto even_set = (*even_sets)();
      auto odd_set = (*odd_sets)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(even_set.size()));
      REQUIRE(size_stays_in_range(odd_set.size()));
      REQUIRE(integers_stay_in_range(even_set));
      REQUIRE(integers_stay_in_range(odd_set));
      REQUIRE(is_even_set(even_set));
      REQUIRE(is_odd_set(odd_set));
    }

    SECTION("impossible") {
      auto invalid_even_sets = sets.map(to_odd_set)->filter(is_even_set);
      auto invalid_odd_sets = sets.map(to_even_set)->filter(is_odd_set);

      REQUIRE_THROWS_AS((*invalid_even_sets)(),
                        strategies::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_odd_sets)(),
                        strategies::OutOfCycles);
    }
  }
}


TEST_CASE("integers \"Sets\" strategy", "[Sets]") {
  SECTION("unsigned char") {
    check_integers_sets_strategy<unsigned char>();
  }

  SECTION("signed char") {
    check_integers_sets_strategy<signed char>();
  }

  SECTION("short int") {
    check_integers_sets_strategy<short>();
  }

  SECTION("unsigned short int") {
    check_integers_sets_strategy<unsigned short>();
  }

  SECTION("int") {
    check_integers_sets_strategy<int>();
  }

  SECTION("unsigned int") {
    check_integers_sets_strategy<unsigned>();
  }

  SECTION("long int") {
    check_integers_sets_strategy<long>();
  }

  SECTION("unsigned long int") {
    check_integers_sets_strategy<unsigned long>();
  }

  SECTION("long long int") {
    check_integers_sets_strategy<long long>();
  }

  SECTION("unsigned long long int") {
    check_integers_sets_strategy<unsigned long long>();
  }
}
