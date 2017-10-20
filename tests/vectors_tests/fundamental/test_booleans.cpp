#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/booleans.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/characters.h"
#include "../../../cauldron/strings.h"
#include "../../../cauldron/vectors.h"
#include "../../factories.h"
#include "../../predicates.h"
#include "../../utils.h"


TEST_CASE("booleans \"Vectors\" strategy", "[Vectors]") {
  auto is_false_vector = [&](std::vector<bool> vector) -> bool {
    return std::all_of(vector.begin(),
                       vector.end(),
                       negate);
  };
  auto is_true_vector = [&](std::vector<bool> vector) -> bool {
    return std::all_of(vector.begin(),
                       vector.end(),
                       identity);
  };

  SECTION("single element domain") {
    size_t min_size = 0;
    size_t max_size = constants::max_capacity;
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto true_values = std::make_shared<strategies::Booleans>(1.);
    auto false_values = std::make_shared<strategies::Booleans>(0.);
    strategies::Vectors<bool> true_vectors(sizes,
                                           true_values);
    strategies::Vectors<bool> false_vectors(sizes,
                                            false_values);

    auto true_vector = true_vectors();
    auto false_vector = false_vectors();
    auto stays_in_range = in_range_checker<size_t>(min_size,
                                                   max_size);

    REQUIRE(stays_in_range(true_vector.size()));
    REQUIRE(stays_in_range(false_vector.size()));
    REQUIRE(is_true_vector(true_vector));
    REQUIRE(is_false_vector(false_vector));
  }

  SECTION("filtration") {
    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty vector.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = sufficient_capacity(1, 2, // true or false
                                          strategies::MAX_CYCLES);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto booleans = std::make_shared<strategies::Booleans>();
    strategies::Vectors<bool> booleans_vectors(sizes,
                                               booleans);

    SECTION("truthfulness") {
      auto true_vectors = booleans_vectors.filter(is_true_vector);
      auto false_vectors = booleans_vectors.filter(is_false_vector);

      auto true_vector = (*true_vectors)();
      auto false_vector = (*false_vectors)();
      auto stays_in_range = in_range_checker(min_size,
                                             max_size);

      REQUIRE(stays_in_range(true_vector.size()));
      REQUIRE(stays_in_range(false_vector.size()));
      REQUIRE(is_true_vector(true_vector));
      REQUIRE(is_false_vector(false_vector));
    }

    SECTION("impossible") {
      auto invalid_vectors =
          booleans_vectors.filter(is_true_vector)->filter(is_false_vector);
      REQUIRE_THROWS_AS((*invalid_vectors)(),
                        strategies::OutOfCycles);
    }
  }

  SECTION("mapping") {
    strategies::Converter<std::vector<bool>> to_false_vector(
        [&](const std::vector<bool> &vector) -> std::vector<bool> {
          auto result = std::vector<bool>(vector.size());
          std::fill(result.begin(),
                    result.end(),
                    false);
          return result;
        });
    strategies::Converter<std::vector<bool>> to_true_vector(
        [&](const std::vector<bool> &vector) -> std::vector<bool> {
          auto result = std::vector<bool>(vector.size());
          std::fill(result.begin(),
                    result.end(),
                    true);
          return result;
        });

    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty vector.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = constants::max_capacity;
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto booleans = std::make_shared<strategies::Booleans>();
    strategies::Vectors<bool> booleans_vectors(sizes,
                                               booleans);

    SECTION("truthfulness") {
      auto false_vectors = booleans_vectors.map(to_false_vector);
      auto true_vectors = booleans_vectors.map(to_true_vector);

      auto false_vector = (*false_vectors)();
      auto true_vector = (*true_vectors)();
      auto stays_in_range = in_range_checker(min_size,
                                             max_size);

      REQUIRE(stays_in_range(false_vector.size()));
      REQUIRE(stays_in_range(true_vector.size()));
      REQUIRE(is_false_vector(false_vector));
      REQUIRE(is_true_vector(true_vector));
    }

    SECTION("impossible") {
      auto invalid_false_vectors =
          booleans_vectors.map(to_false_vector)->filter(is_true_vector);
      auto invalid_true_vectors =
          booleans_vectors.map(to_false_vector)->filter(is_true_vector);
      REQUIRE_THROWS_AS((*invalid_false_vectors)(),
                        strategies::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_true_vectors)(),
                        strategies::OutOfCycles);
    }
  }
}
