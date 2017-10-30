#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/floats.h"
#include "../../../cauldron/sets.h"
#include "../../factories.h"
#include "../../predicates.h"
#include "../../operators.h"
#include "../../utils.h"


template<typename T>
static void check_strategy() {
  cauldron::Requirement<std::set<T>> is_positive_set(
      [&](std::set<T> set) -> bool {
        return std::all_of(set.begin(),
                           set.end(),
                           positive<T>);
      });
  cauldron::Requirement<std::set<T>> is_non_positive_set(
      [&](std::set<T> set) -> bool {
        return std::all_of(set.begin(),
                           set.end(),
                           non_positive<T>);
      });

  T min_possible_number = std::numeric_limits<T>::lowest();
  T min_possible_positive_number = std::numeric_limits<T>::min();
  T max_possible_number = std::numeric_limits<T>::max();

  static std::random_device random_device;

  auto distribution = std::uniform_real_distribution<T>(
      min_possible_number,
      -min_possible_positive_number);
  T min_number = distribution(random_device);
  T max_number = max_possible_number + min_number;
  auto numbers = std::make_shared<cauldron::Floats<T>>(min_number,
                                                       max_number);
  auto number_stays_in_range = in_range_checker<T>(min_number,
                                                   max_number);
  auto numbers_stay_in_range = [&](const std::set<T> &set) -> bool {
    return std::all_of(set.begin(),
                       set.end(),
                       number_stays_in_range);
  };

  SECTION("stays in range") {
    static size_t min_size = 0;
    static size_t max_size = constants::max_capacity;
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    static const auto sizes =
        std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                     max_size);
    cauldron::Sets<T> numbers_sets(sizes,
                                   numbers);

    auto set = numbers_sets();

    REQUIRE(size_stays_in_range(set.size()));
    REQUIRE(numbers_stay_in_range(set));
  }

  SECTION("union") {
    static size_t min_size = 0;
    static size_t max_size = constants::max_capacity;
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    static const auto sizes =
        std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                     max_size);
    cauldron::Sets<T> numbers_sets(sizes,
                                   numbers);
    auto still_numbers_sets = numbers_sets || numbers_sets;

    auto set = still_numbers_sets();

    REQUIRE(size_stays_in_range(set.size()));
    REQUIRE(numbers_stay_in_range(set));
  }

  SECTION("filtration") {
    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    static size_t min_size = constants::min_capacity;
    static size_t max_size = sufficient_capacity(1, 2, // non- or positive
                                                 cauldron::MAX_CYCLES);
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    cauldron::Sets<T> sets(sizes,
                           numbers);

    SECTION("sign") {
      auto positive_sets = sets.filter(is_positive_set);
      auto non_positive_sets = sets.filter(is_non_positive_set);

      auto positive_set = (*positive_sets)();
      auto non_positive_set = (*non_positive_sets)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(positive_set.size()));
      REQUIRE(size_stays_in_range(non_positive_set.size()));
      REQUIRE(numbers_stay_in_range(positive_set));
      REQUIRE(numbers_stay_in_range(non_positive_set));
      REQUIRE(is_positive_set(positive_set));
      REQUIRE(is_non_positive_set(non_positive_set));
    }

    SECTION("impossible") {
      auto invalid_sets =
          sets.filter(is_positive_set)->filter(is_non_positive_set);

      REQUIRE_THROWS_AS((*invalid_sets)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    auto to_positive = to_positive_operator(max_number);
    auto to_non_positive = to_non_positive_operator(min_number);
    cauldron::Converter<std::set<T>> to_positive_set(
        [&](const std::set<T> &set) -> std::set<T> {
          std::vector<T> vector;
          vector.reserve(set.size());
          std::transform(set.begin(),
                         set.end(),
                         std::back_inserter(vector),
                         to_positive);
          return std::set<T>(vector.begin(),
                             vector.end());
        });
    cauldron::Converter<std::set<T>> to_non_positive_set(
        [&](const std::set<T> &set) -> std::set<T> {
          std::vector<T> vector;
          vector.reserve(set.size());
          std::transform(set.begin(),
                         set.end(),
                         std::back_inserter(vector),
                         to_non_positive);
          return std::set<T>(vector.begin(),
                             vector.end());
        });

    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    static size_t min_size = constants::min_capacity;
    static size_t max_size = sufficient_capacity(1, 2, // non- or positive
                                                 cauldron::MAX_CYCLES);
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    cauldron::Sets<T> sets(sizes,
                           numbers);

    SECTION("sign") {
      auto positive_sets = sets.map(to_positive_set);
      auto non_positive_sets = sets.map(to_non_positive_set);

      auto positive_set = (*positive_sets)();
      auto non_positive_set = (*non_positive_sets)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(positive_set.size()));
      REQUIRE(size_stays_in_range(non_positive_set.size()));
      REQUIRE(numbers_stay_in_range(positive_set));
      REQUIRE(numbers_stay_in_range(non_positive_set));
      REQUIRE(is_positive_set(positive_set));
      REQUIRE(is_non_positive_set(non_positive_set));
    }

    SECTION("impossible") {
      auto invalid_positive_sets =
          sets.map(to_non_positive_set)->filter(is_positive_set);
      auto invalid_non_positive_sets =
          sets.map(to_positive_set)->filter(is_non_positive_set);

      REQUIRE_THROWS_AS((*invalid_positive_sets)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_non_positive_sets)(),
                        cauldron::OutOfCycles);
    }
  }
}


TEST_CASE("floats \"Sets\" strategy", "[Sets]") {
  SECTION("float") {
    check_strategy<float>();
  }

  SECTION("double") {
    check_strategy<double>();
  }

  SECTION("long double") {
    check_strategy<long double>();
  }
}
