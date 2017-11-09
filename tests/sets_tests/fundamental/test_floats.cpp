#include <algorithm>

#include <catch.hpp>
#include <cauldron/just.h>
#include <cauldron/integers.h>
#include <cauldron/floats.h>
#include <cauldron/sets.h>

#include <tests/factories.h>
#include <tests/predicates.h>
#include <tests/operators.h>
#include <tests/ordered_pair.h>
#include <tests/utils.h>


template<typename Number>
static void check_strategy() {
  cauldron::Requirement<std::set<Number>> is_positive_set(
      [&](std::set<Number> set) -> bool {
        return std::all_of(set.begin(),
                           set.end(),
                           positive<Number>);
      });
  cauldron::Requirement<std::set<Number>> is_non_positive_set(
      [&](std::set<Number> set) -> bool {
        return std::all_of(set.begin(),
                           set.end(),
                           non_positive<Number>);
      });

  Number min_number;
  Number max_number;
  std::tie(min_number, max_number) = ordered_pair(
      std::numeric_limits<Number>::lowest(),
      std::numeric_limits<Number>::max()
  );
  auto number_stays_in_range = in_range_checker<Number>(min_number,
                                                        max_number);
  auto numbers_stay_in_range = [&](const std::set<Number> &set) -> bool {
    return std::all_of(set.begin(),
                       set.end(),
                       number_stays_in_range);
  };
  auto numbers = std::make_shared<cauldron::Floats<Number>>(min_number,
                                                            max_number);

  SECTION("stays in range") {
    static size_t min_size = 0;
    static size_t max_size = constants::max_capacity;
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    static const auto sizes =
        std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                     max_size);
    cauldron::Sets<Number> numbers_sets(sizes,
                                        numbers);

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
    static size_t max_size = sufficient_capacity(1, 2, // non- or positive
                                                 cauldron::MAX_CYCLES);
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    cauldron::Sets<Number> sets(sizes,
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
    cauldron::Converter<std::set<Number>> to_positive_set(
        [&](const std::set<Number> &set) -> std::set<Number> {
          std::vector<Number> vector;
          vector.reserve(set.size());
          std::transform(set.begin(),
                         set.end(),
                         std::back_inserter(vector),
                         to_positive);
          return std::set<Number>(vector.begin(),
                                  vector.end());
        });
    cauldron::Converter<std::set<Number>> to_non_positive_set(
        [&](const std::set<Number> &set) -> std::set<Number> {
          std::vector<Number> vector;
          vector.reserve(set.size());
          std::transform(set.begin(),
                         set.end(),
                         std::back_inserter(vector),
                         to_non_positive);
          return std::set<Number>(vector.begin(),
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
    cauldron::Sets<Number> sets(sizes,
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
