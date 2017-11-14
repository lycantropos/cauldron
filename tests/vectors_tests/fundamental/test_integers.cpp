#include <algorithm>

#include <catch.hpp>
#include <cauldron/just.h>
#include <cauldron/booleans.h>
#include <cauldron/integers.h>
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
  cauldron::Requirement<std::vector<Number>> is_even_vector(
      [&](const std::vector<Number> &vector) -> bool {
        return std::all_of(vector.begin(),
                           vector.end(),
                           even<Number>);
      });
  cauldron::Requirement<std::vector<Number>> is_odd_vector(
      [&](const std::vector<Number> &vector) -> bool {
        return std::all_of(vector.begin(),
                           vector.end(),
                           odd<Number>);
      });

  // ``signed char`` is the smallest tested integer type
  static signed char min_integer = std::is_unsigned<Number>() ?
                                   0 : std::numeric_limits<signed char>::min();
  static signed char max_integer = std::numeric_limits<signed char>::max();
  static auto numbers_range = factories::integers_range<int>(min_integer,
                                                             max_integer);

  Number min_number;
  Number max_number;
  std::tie(min_number, max_number) = ordered_pair(
      std::numeric_limits<Number>::min(),
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
  cauldron::Integers<Number> numbers(min_number,
                                     max_number);

  SECTION("single element domain") {
    cauldron::Just<size_t> sizes(1);
    for (Number number: numbers_range) {
      cauldron::Just<Number> same_number(number);
      cauldron::Vectors<Number> same_number_vectors(sizes,
                                                    same_number);

      auto vector = same_number_vectors();

      REQUIRE(vector == std::vector<Number>{number});
    }
  }

  SECTION("multiple elements domain") {
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
    static size_t max_size = sufficient_capacity(1, 2, // odd or even
                                                 cauldron::MAX_CYCLES);
    cauldron::Integers<size_t> sizes(min_size,
                                     max_size);
    cauldron::Vectors<Number> vectors(sizes,
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
    cauldron::Converter<Number> to_even(to_even_operator(min_number,
                                                         max_number));
    cauldron::Converter<Number> to_odd(to_odd_operator(min_number,
                                                       max_number));
    cauldron::Converter<std::vector<Number>> to_even_vector(
        [&](const std::vector<Number> &vector) -> std::vector<Number> {
          std::vector<Number> result(vector.size());
          std::transform(vector.begin(),
                         vector.end(),
                         result.begin(),
                         to_even);
          return result;
        });
    cauldron::Converter<std::vector<Number>> to_odd_vector(
        [&](const std::vector<Number> &vector) -> std::vector<Number> {
          std::vector<Number> result(vector.size());
          std::transform(vector.begin(),
                         vector.end(),
                         result.begin(),
                         to_odd);
          return result;
        });

    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty vector.
     */
    static size_t min_size = constants::min_capacity;
    static size_t max_size = sufficient_capacity(1, 2, // odd or even
                                                 cauldron::MAX_CYCLES);
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    cauldron::Integers<size_t> sizes(min_size,
                                     max_size);
    cauldron::Vectors<Number> vectors(sizes,
                                      numbers);

    SECTION("parity") {
      auto even_vectors = vectors.map(to_even_vector);
      auto odd_vectors = vectors.map(to_odd_vector);

      auto even_vector = (*even_vectors)();
      auto odd_vector = (*odd_vectors)();

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
