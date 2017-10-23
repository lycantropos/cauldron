#include <catch.hpp>
#include "../../../cauldron/booleans.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/sets.h"
#include "../../predicates.h"
#include "../../globals.h"


TEST_CASE("booleans \"Sets\" strategy", "[Sets]") {
  auto is_false_set = [&](std::set<bool> set) -> bool {
    return std::all_of(set.begin(),
                       set.end(),
                       negate);
  };
  auto is_true_set = [&](std::set<bool> set) -> bool {
    return std::all_of(set.begin(),
                       set.end(),
                       identity);
  };

  SECTION("single element domain") {
    size_t min_size = 0;
    size_t max_size = 1;
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto true_values = std::make_shared<strategies::Booleans>(1.);
    auto false_values = std::make_shared<strategies::Booleans>(0.);
    strategies::Sets<bool> true_sets(sizes,
                                     true_values);
    strategies::Sets<bool> false_sets(sizes,
                                      false_values);

    auto true_set = true_sets();
    auto false_set = false_sets();
    auto stays_in_range = in_range_checker<size_t>(min_size,
                                                   max_size);

    REQUIRE(stays_in_range(true_set.size()));
    REQUIRE(stays_in_range(false_set.size()));
    REQUIRE(is_true_set(true_set));
    REQUIRE(is_false_set(false_set));
  }

  SECTION("filtration") {
    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = 2; // true or false;
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto booleans = std::make_shared<strategies::Booleans>();
    strategies::Sets<bool> booleans_sets(sizes,
                                         booleans);

    SECTION("truthfulness") {
      auto true_sets = booleans_sets.filter(is_true_set);
      auto false_sets = booleans_sets.filter(is_false_set);

      auto true_set = (*true_sets)();
      auto false_set = (*false_sets)();
      auto stays_in_range = in_range_checker(min_size,
                                             max_size);

      REQUIRE(stays_in_range(true_set.size()));
      REQUIRE(stays_in_range(false_set.size()));
      REQUIRE(is_true_set(true_set));
      REQUIRE(is_false_set(false_set));
    }

    SECTION("impossible") {
      auto invalid_sets =
          booleans_sets.filter(is_true_set)->filter(is_false_set);
      REQUIRE_THROWS_AS((*invalid_sets)(),
                        strategies::OutOfCycles);
    }
  }

  SECTION("mapping") {
    strategies::Converter<std::set<bool>> to_false_set(
        [&](const std::set<bool> &set) -> std::set<bool> {
          std::vector<bool> vector(set.size());
          std::fill(vector.begin(),
                    vector.end(),
                    false);
          return std::set<bool>(vector.begin(),
                                vector.end());
        });
    strategies::Converter<std::set<bool>> to_true_set(
        [&](const std::set<bool> &set) -> std::set<bool> {
          std::vector<bool> vector(set.size());
          std::fill(vector.begin(),
                    vector.end(),
                    true);
          return std::set<bool>(vector.begin(),
                                vector.end());
        });

    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = 2; // true or false;
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto booleans = std::make_shared<strategies::Booleans>();
    strategies::Sets<bool> booleans_sets(sizes,
                                         booleans);

    SECTION("truthfulness") {
      auto false_sets = booleans_sets.map(to_false_set);
      auto true_sets = booleans_sets.map(to_true_set);

      auto false_set = (*false_sets)();
      auto true_set = (*true_sets)();
      auto stays_in_range = in_range_checker(min_size,
                                             max_size);

      REQUIRE(stays_in_range(false_set.size()));
      REQUIRE(stays_in_range(true_set.size()));
      REQUIRE(is_false_set(false_set));
      REQUIRE(is_true_set(true_set));
    }

    SECTION("impossible") {
      auto invalid_false_sets =
          booleans_sets.map(to_false_set)->filter(is_true_set);
      auto invalid_true_sets =
          booleans_sets.map(to_false_set)->filter(is_true_set);
      REQUIRE_THROWS_AS((*invalid_false_sets)(),
                        strategies::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_true_sets)(),
                        strategies::OutOfCycles);
    }
  }
}
