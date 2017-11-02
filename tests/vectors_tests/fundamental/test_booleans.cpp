#include <catch.hpp>
#include <cauldron/just.h>
#include <cauldron/booleans.h>
#include <cauldron/integers.h>
#include <cauldron/characters.h>
#include <cauldron/strings.h>
#include <cauldron/vectors.h>
#include <tests/factories.h>
#include <tests/predicates.h>
#include <tests/utils.h>


TEST_CASE("booleans \"Vectors\" strategy", "[Vectors]") {
  cauldron::Requirement<std::vector<bool>> is_false_vector(
      [&](const std::vector<bool> &vector) -> bool {
        return std::all_of(vector.begin(),
                           vector.end(),
                           negate);
      });
  cauldron::Requirement<std::vector<bool>> is_true_vector(
      [&](const std::vector<bool> &vector) -> bool {
        return std::all_of(vector.begin(),
                           vector.end(),
                           identity);
      });

  SECTION("single element domain") {
    size_t min_size = 0;
    size_t max_size = constants::max_capacity;
    auto stays_in_range = in_range_checker<size_t>(min_size,
                                                   max_size);
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    auto false_values = std::make_shared<cauldron::Booleans>(0.);
    auto true_values = std::make_shared<cauldron::Booleans>(1.);
    cauldron::Vectors<bool> false_vectors(sizes,
                                          false_values);
    cauldron::Vectors<bool> true_vectors(sizes,
                                         true_values);

    auto false_vector = false_vectors();
    auto true_vector = true_vectors();

    REQUIRE(stays_in_range(false_vector.size()));
    REQUIRE(stays_in_range(true_vector.size()));
    REQUIRE(is_false_vector(false_vector));
    REQUIRE(is_true_vector(true_vector));
  }

  SECTION("union") {
    size_t min_size = 0;
    size_t max_size = constants::max_capacity;
    auto stays_in_range = in_range_checker<size_t>(min_size,
                                                   max_size);
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    auto false_values = std::make_shared<cauldron::Booleans>(0.);
    auto true_values = std::make_shared<cauldron::Booleans>(1.);
    cauldron::Vectors<bool> false_vectors(sizes,
                                          false_values);
    cauldron::Vectors<bool> true_vectors(sizes,
                                         true_values);
    auto still_false_vectors = false_vectors || false_vectors;
    auto still_true_vectors = true_vectors || true_vectors;

    auto false_vector = still_false_vectors();
    auto true_vector = still_true_vectors();

    REQUIRE(stays_in_range(false_vector.size()));
    REQUIRE(stays_in_range(true_vector.size()));
    REQUIRE(is_false_vector(false_vector));
    REQUIRE(is_true_vector(true_vector));
  }

  SECTION("filtration") {
    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty vector.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = sufficient_capacity(1, 2, // true or false
                                          cauldron::MAX_CYCLES);
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    auto booleans = std::make_shared<cauldron::Booleans>();
    cauldron::Vectors<bool> booleans_vectors(sizes,
                                             booleans);

    SECTION("truthfulness") {
      auto false_vectors = booleans_vectors.filter(is_false_vector);
      auto true_vectors = booleans_vectors.filter(is_true_vector);

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
      auto invalid_vectors =
          booleans_vectors.filter(is_false_vector)->filter(is_true_vector);
      REQUIRE_THROWS_AS((*invalid_vectors)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    cauldron::Converter<std::vector<bool>> to_false_vector(
        [&](const std::vector<bool> &vector) -> std::vector<bool> {
          auto result = std::vector<bool>(vector.size());
          std::fill(result.begin(),
                    result.end(),
                    false);
          return result;
        });
    cauldron::Converter<std::vector<bool>> to_true_vector(
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
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    auto booleans = std::make_shared<cauldron::Booleans>();
    cauldron::Vectors<bool> booleans_vectors(sizes,
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
          booleans_vectors.map(to_true_vector)->filter(is_false_vector);
      auto invalid_true_vectors =
          booleans_vectors.map(to_false_vector)->filter(is_true_vector);

      REQUIRE_THROWS_AS((*invalid_false_vectors)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_true_vectors)(),
                        cauldron::OutOfCycles);
    }
  }
}
