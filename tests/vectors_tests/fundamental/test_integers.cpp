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
#include "../../operators.h"


template<typename T>
static void check_integers_vectors_strategy() {
  auto is_even_vector = [&](std::vector<T> vector) -> bool {
    return std::all_of(vector.begin(),
                       vector.end(),
                       even<T>);
  };
  auto is_odd_vector = [&](std::vector<T> vector) -> bool {
    return std::all_of(vector.begin(),
                       vector.end(),
                       odd<T>);
  };

  T min_value = std::numeric_limits<T>::min();
  T max_value = std::numeric_limits<T>::max();
  auto integers = std::make_shared<strategies::Integers<T>>(min_value,
                                                            max_value);
  auto integer_stays_in_range = in_range_checker<T>(min_value,
                                                    max_value);
  auto integers_stay_in_range =
      [&](const std::vector<T> &integers_vector) -> bool {
        return std::all_of(integers_vector.begin(),
                           integers_vector.end(),
                           integer_stays_in_range);
      };

  SECTION("single element domain") {
    // ``signed char`` is the smallest tested integer type
    static int min_integer = std::numeric_limits<signed char>::min();
    static int max_integer = std::numeric_limits<signed char>::max();
    static const auto integers_range = factories::integers_range<int>(
        min_integer,
        max_integer);
    strategies::Just<size_t> ones(1);
    for (T integer: integers_range) {
      auto single_integer_vector = std::vector<T>{integer};
      strategies::Just<T> same_integer(integer);
      strategies::Vectors<T> same_integer_vectors(
          std::make_shared<strategies::Just<size_t>>(ones),
          std::make_shared<strategies::Just<T>>(same_integer));

      auto vector = same_integer_vectors();

      bool vectors_are_equal = vector == single_integer_vector;
      REQUIRE(vectors_are_equal);
    }
  }

  SECTION("multiple elements domain") {
    static size_t min_size = 0;
    static size_t max_size = constants::max_capacity;
    static const std::shared_ptr<strategies::Integers<size_t>> &sizes =
        std::make_shared<strategies::Integers<size_t>>(min_size,
                                                       max_size);
    strategies::Vectors<T> integers_vectors(sizes,
                                            integers);
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    auto vector = integers_vectors();

    REQUIRE(size_stays_in_range(vector.size()));
    REQUIRE(integers_stay_in_range(vector));
  }

  SECTION("filtration") {
    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty vector.
     */
    static size_t min_size = constants::min_capacity;
    static size_t max_size = sufficient_capacity(1, 2, // odd or even
                                                 strategies::MAX_CYCLES);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    strategies::Vectors<T> vectors(sizes,
                                   integers);

    SECTION("parity") {
      auto even_vectors = vectors.filter(is_even_vector);
      auto odd_vectors = vectors.filter(is_odd_vector);

      auto even_vector = (*even_vectors)();
      auto odd_vector = (*odd_vectors)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(even_vector.size()));
      REQUIRE(size_stays_in_range(odd_vector.size()));
      REQUIRE(integers_stay_in_range(even_vector));
      REQUIRE(integers_stay_in_range(odd_vector));
      REQUIRE(is_even_vector(even_vector));
      REQUIRE(is_odd_vector(odd_vector));
    }

    SECTION("impossible") {
      auto invalid_vectors =
          vectors.filter(is_even_vector)->filter(is_odd_vector);

      REQUIRE_THROWS_AS((*invalid_vectors)(),
                        strategies::OutOfCycles);
    }
  }

  SECTION("mapping") {
    strategies::Converter<std::vector<T>> to_even_vector(
        [&](const std::vector<T> &vector) -> std::vector<T> {
          auto result = std::vector<T>(vector.size());
          std::transform(vector.begin(),
                         vector.end(),
                         result.begin(),
                         to_even<T>);
          return result;
        });
    strategies::Converter<std::vector<T>> to_odd_vector(
        [&](const std::vector<T> &vector) -> std::vector<T> {
          auto result = std::vector<T>(vector.size());
          std::transform(vector.begin(),
                         vector.end(),
                         result.begin(),
                         to_odd<T>);
          return result;
        });

    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty vector.
     */
    static size_t min_size = constants::min_capacity;
    static size_t max_size = sufficient_capacity(1, 2, // odd or even
                                                 strategies::MAX_CYCLES);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    strategies::Vectors<T> vectors(sizes,
                                   integers);

    SECTION("parity") {
      auto even_vectors = vectors.map(to_even_vector);
      auto odd_vectors = vectors.map(to_odd_vector);

      auto even_vector = (*even_vectors)();
      auto odd_vector = (*odd_vectors)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(even_vector.size()));
      REQUIRE(size_stays_in_range(odd_vector.size()));
      REQUIRE(integers_stay_in_range(even_vector));
      REQUIRE(integers_stay_in_range(odd_vector));
      REQUIRE(is_even_vector(even_vector));
      REQUIRE(is_odd_vector(odd_vector));
    }

    SECTION("impossible") {
      auto invalid_even_vectors =
          vectors.map(to_odd_vector)->filter(is_even_vector);
      auto invalid_odd_vectors =
          vectors.map(to_even_vector)->filter(is_odd_vector);

      REQUIRE_THROWS_AS((*invalid_even_vectors)(),
                        strategies::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_odd_vectors)(),
                        strategies::OutOfCycles);
    }
  }
}


TEST_CASE("\"vectors\" strategy", "[vectors]") {
  SECTION("unsigned char") {
    check_integers_vectors_strategy<unsigned char>();
  }

  SECTION("signed char") {
    check_integers_vectors_strategy<signed char>();
  }

  SECTION("short int") {
    check_integers_vectors_strategy<short>();
  }

  SECTION("unsigned short int") {
    check_integers_vectors_strategy<unsigned short>();
  }

  SECTION("int") {
    check_integers_vectors_strategy<int>();
  }

  SECTION("unsigned int") {
    check_integers_vectors_strategy<unsigned>();
  }

  SECTION("long int") {
    check_integers_vectors_strategy<long>();
  }

  SECTION("unsigned long int") {
    check_integers_vectors_strategy<unsigned long>();
  }

  SECTION("long long int") {
    check_integers_vectors_strategy<long long>();
  }

  SECTION("unsigned long long int") {
    check_integers_vectors_strategy<unsigned long long>();
  }
}
