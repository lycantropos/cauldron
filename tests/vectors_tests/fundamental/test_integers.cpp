#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/booleans.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/characters.h"
#include "../../../cauldron/strings.h"
#include "../../../cauldron/vectors.h"
#include "../../factories.h"
#include "../../predicates.h"
#include "../../operators.h"
#include "../../utils.h"


template<typename T>
static void check_strategy() {
  cauldron::Requirement<std::vector<T>> is_even_vector(
      [&](const std::vector<T> &vector) -> bool {
        return std::all_of(vector.begin(),
                           vector.end(),
                           even<T>);
      });
  cauldron::Requirement<std::vector<T>> is_odd_vector(
      [&](const std::vector<T> &vector) -> bool {
        return std::all_of(vector.begin(),
                           vector.end(),
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
  auto numbers_stay_in_range =
      [&](const std::vector<T> &numbers_vector) -> bool {
        return std::all_of(numbers_vector.begin(),
                           numbers_vector.end(),
                           number_stays_in_range);
      };

  SECTION("single element domain") {
    cauldron::Just<size_t> ones(1);
    for (T number: numbers_range) {
      cauldron::Just<T> same_number(number);
      cauldron::Vectors<T> same_number_vectors(
          std::make_shared<cauldron::Just<size_t>>(ones),
          std::make_shared<cauldron::Just<T>>(same_number));

      auto vector = same_number_vectors();

      REQUIRE(vector == std::vector<T>{number});
    }
  }

  SECTION("multiple elements domain") {
    static size_t min_size = 0;
    static size_t max_size = constants::max_capacity;
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    static const auto sizes =
        std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                     max_size);
    cauldron::Vectors<T> numbers_vectors(sizes,
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
    static size_t max_size = sufficient_capacity(1, 2, // odd or even
                                                 cauldron::MAX_CYCLES);
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    cauldron::Vectors<T> vectors(sizes,
                                 numbers);

    SECTION("parity") {
      auto even_vectors = vectors.filter(is_even_vector);
      auto odd_vectors = vectors.filter(is_odd_vector);

      auto even_vector = (*even_vectors)();
      auto odd_vector = (*odd_vectors)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(even_vector.size()));
      REQUIRE(size_stays_in_range(odd_vector.size()));
      REQUIRE(numbers_stay_in_range(even_vector));
      REQUIRE(numbers_stay_in_range(odd_vector));
      REQUIRE(is_even_vector(even_vector));
      REQUIRE(is_odd_vector(odd_vector));
    }

    SECTION("impossible") {
      auto invalid_vectors =
          vectors.filter(is_even_vector)->filter(is_odd_vector);

      REQUIRE_THROWS_AS((*invalid_vectors)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    cauldron::Converter<std::vector<T>> to_even_vector(
        [&](const std::vector<T> &vector) -> std::vector<T> {
          auto result = std::vector<T>(vector.size());
          std::transform(vector.begin(),
                         vector.end(),
                         result.begin(),
                         to_even<T>);
          return result;
        });
    cauldron::Converter<std::vector<T>> to_odd_vector(
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
                                                 cauldron::MAX_CYCLES);
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    cauldron::Vectors<T> vectors(sizes,
                                 numbers);

    SECTION("parity") {
      auto even_vectors = vectors.map(to_even_vector);
      auto odd_vectors = vectors.map(to_odd_vector);

      auto even_vector = (*even_vectors)();
      auto odd_vector = (*odd_vectors)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(even_vector.size()));
      REQUIRE(size_stays_in_range(odd_vector.size()));
      REQUIRE(numbers_stay_in_range(even_vector));
      REQUIRE(numbers_stay_in_range(odd_vector));
      REQUIRE(is_even_vector(even_vector));
      REQUIRE(is_odd_vector(odd_vector));
    }

    SECTION("impossible") {
      auto invalid_even_vectors =
          vectors.map(to_odd_vector)->filter(is_even_vector);
      auto invalid_odd_vectors =
          vectors.map(to_even_vector)->filter(is_odd_vector);

      REQUIRE_THROWS_AS((*invalid_even_vectors)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_odd_vectors)(),
                        cauldron::OutOfCycles);
    }
  }
}


TEST_CASE("integers \"Vectors\" strategy", "[Vectors]") {
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
