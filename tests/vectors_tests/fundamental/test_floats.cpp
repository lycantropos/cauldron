#include <algorithm>

#include <catch.hpp>
#include <cauldron/just.h>
#include <cauldron/booleans.h>
#include <cauldron/integers.h>
#include <cauldron/floats.h>
#include <cauldron/characters.h>
#include <cauldron/strings.h>
#include <cauldron/vectors.h>

#include <tests/factories.h>
#include <tests/predicates.h>
#include <tests/operators.h>
#include <tests/ordered_pair.h>
#include <tests/utils.h>


template<typename Number>
static void check_strategy() {
  cauldron::Requirement<std::vector<Number>> is_positive_vector(
      [&](const std::vector<Number> &vector) -> bool {
        return std::all_of(vector.begin(),
                           vector.end(),
                           positive<Number>);
      });
  cauldron::Requirement<std::vector<Number>> is_non_positive_vector(
      [&](const std::vector<Number> &vector) -> bool {
        return std::all_of(vector.begin(),
                           vector.end(),
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
  auto numbers_stay_in_range =
      [&](const std::vector<Number> &numbers_vector) -> bool {
        return std::all_of(numbers_vector.begin(),
                           numbers_vector.end(),
                           number_stays_in_range);
      };
  cauldron::Floats<Number> numbers(min_number,
                                   max_number);

  SECTION("stays in range") {
    static size_t min_size = 0;
    static size_t max_size = constants::max_capacity;
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    static const cauldron::Integers<size_t> sizes(min_size,
                                                  max_size);
    cauldron::Vectors<Number> numbers_vectors(sizes,
                                              numbers);

    auto vector = numbers_vectors();

    REQUIRE(size_stays_in_range(vector.size()));
    REQUIRE(numbers_stay_in_range(vector));
  }

  SECTION("filtration") {
    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty vector.
     */
    static size_t min_size = constants::min_capacity;
    static size_t max_size = sufficient_capacity(1, 2, // non- or positive
                                                 cauldron::MAX_CYCLES);
    static const cauldron::Integers<size_t> sizes(min_size,
                                                  max_size);
    cauldron::Vectors<Number> vectors(sizes,
                                      numbers);

    SECTION("sign") {
      auto positive_vectors = vectors.filter(is_positive_vector);
      auto non_positive_vectors = vectors.filter(is_non_positive_vector);

      auto positive_vector = (*positive_vectors)();
      auto non_positive_vector = (*non_positive_vectors)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(positive_vector.size()));
      REQUIRE(size_stays_in_range(non_positive_vector.size()));
      REQUIRE(numbers_stay_in_range(positive_vector));
      REQUIRE(numbers_stay_in_range(non_positive_vector));
      REQUIRE(is_positive_vector(positive_vector));
      REQUIRE(is_non_positive_vector(non_positive_vector));
    }

    SECTION("impossible") {
      auto invalid_vectors =
          vectors.filter(is_positive_vector)->filter(is_non_positive_vector);

      REQUIRE_THROWS_AS((*invalid_vectors)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    auto to_positive = to_positive_operator(max_number);
    auto to_non_positive = to_non_positive_operator(min_number);
    cauldron::Converter<std::vector<Number>> to_positive_vector(
        [&](const std::vector<Number> &vector) -> std::vector<Number> {
          std::vector<Number> result(vector.size());
          std::transform(vector.begin(),
                         vector.end(),
                         result.begin(),
                         to_positive);
          return result;
        });
    cauldron::Converter<std::vector<Number>> to_non_positive_vector(
        [&](const std::vector<Number> &vector) -> std::vector<Number> {
          std::vector<Number> result(vector.size());
          std::transform(vector.begin(),
                         vector.end(),
                         result.begin(),
                         to_non_positive);
          return result;
        });

    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty vector.
     */
    static size_t min_size = constants::min_capacity;
    static size_t
        max_size = sufficient_capacity(1, 2, // non_positive or positive
                                       cauldron::MAX_CYCLES);
    cauldron::Integers<size_t> sizes(min_size,
                                     max_size);
    cauldron::Vectors<Number> vectors(sizes,
                                      numbers);

    SECTION("sign") {
      auto positive_vectors = vectors.map(to_positive_vector);
      auto non_positive_vectors = vectors.map(to_non_positive_vector);

      auto positive_vector = (*positive_vectors)();
      auto non_positive_vector = (*non_positive_vectors)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(positive_vector.size()));
      REQUIRE(size_stays_in_range(non_positive_vector.size()));
      REQUIRE(numbers_stay_in_range(positive_vector));
      REQUIRE(numbers_stay_in_range(non_positive_vector));
      REQUIRE(is_positive_vector(positive_vector));
      REQUIRE(is_non_positive_vector(non_positive_vector));
    }

    SECTION("impossible") {
      auto invalid_positive_vectors =
          vectors.map(to_non_positive_vector)->filter(is_positive_vector);
      auto invalid_non_positive_vectors =
          vectors.map(to_positive_vector)->filter(is_non_positive_vector);

      REQUIRE_THROWS_AS((*invalid_positive_vectors)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_non_positive_vectors)(),
                        cauldron::OutOfCycles);
    }
  }
}


TEST_CASE("floats \"Vectors\" strategy", "[Vectors]") {
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
