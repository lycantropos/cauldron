#include <catch.hpp>
#include <iostream>
#include "../cauldron/just.h"
#include "../cauldron/booleans.h"
#include "../cauldron/integers.h"
#include "../cauldron/characters.h"
#include "../cauldron/vectors.h"
#include "predicates.h"
#include "factories.h"
#include "utils.h"
#include "../cauldron/strings.h"


template<typename T>
bool is_vector_from_domain(const std::vector<T> &vector,
                           const std::vector<T> &domain) {
  auto is_object_from_domain = [=](T object) -> bool {
    return is_object_in_vector<T>(object, domain);
  };
  return std::all_of(vector.begin(),
                     vector.end(),
                     is_object_from_domain);
}


template<typename T>
static void check_integers_vectors_strategy() {
  static size_t min_size = constants::min_capacity;

  T min_value = std::numeric_limits<T>::min();
  T max_value = std::numeric_limits<T>::max();
  const std::shared_ptr<strategies::Integers<T>> &elements =
      std::make_shared<strategies::Integers<T>>(min_value,
                                                max_value);

  SECTION("single element domain") {
    // ``signed char`` is the smallest tested integer type
    static int min_integer = std::numeric_limits<signed char>::min();
    static int max_integer = std::numeric_limits<signed char>::max();
    static const auto integers = factories::integers_range<int>(min_integer,
                                                                max_integer);
    strategies::Just<size_t> ones(1);
    for (T integer: integers) {
      auto single_element_vector = std::vector<T>{integer};
      strategies::Just<T> same_integer(integer);
      strategies::Vectors<T> same_integer_vectors(
          std::make_shared<strategies::Just<size_t>>(ones),
          std::make_shared<strategies::Just<T>>(same_integer));

      auto vector = same_integer_vectors();

      bool vectors_are_equal = vector == single_element_vector;
      REQUIRE(vectors_are_equal);
    }
  }

  SECTION("multiple elements domain") {
    static size_t max_size = constants::max_capacity;
    static const std::shared_ptr<strategies::Integers<size_t>> &sizes =
        std::make_shared<strategies::Integers<size_t>>(min_size,
                                                       max_size);
    strategies::Vectors<T> vectors(sizes,
                                   elements);
    auto stays_in_range = in_range_checker<T>(min_value,
                                              max_value);

    auto vector = vectors();

    REQUIRE(min_size <= vector.size() <= max_size);
    REQUIRE(std::all_of(vector.begin(),
                        vector.end(),
                        stays_in_range));
  }

  SECTION("filtration") {
    static size_t max_size = sufficient_capacity(1, 2, // odd or even
                                                 strategies::MAX_CYCLES);
    auto all_even = [&](std::vector<T> vector) -> bool {
      return std::all_of(vector.begin(),
                         vector.end(),
                         even<T>);
    };
    auto all_odd = [&](std::vector<T> vector) -> bool {
      return std::all_of(vector.begin(),
                         vector.end(),
                         odd<T>);
    };
    static const std::shared_ptr<strategies::Integers<size_t>> &sizes =
        std::make_shared<strategies::Integers<size_t>>(min_size,
                                                       max_size);
    strategies::Vectors<T> vectors(sizes,
                                   elements);

    SECTION("parity") {
      auto all_even_vectors = vectors.filter(all_even);
      auto all_odd_vectors = vectors.filter(all_odd);

      auto all_even_vector = (*all_even_vectors)();
      auto all_odd_vector = (*all_odd_vectors)();
      auto stays_in_range = in_range_checker<size_t>(min_size,
                                                     max_size);

      REQUIRE(stays_in_range(all_even_vector.size()));
      REQUIRE(stays_in_range(all_odd_vector.size()));
      REQUIRE(all_even(all_even_vector));
      REQUIRE(all_odd(all_odd_vector));
    }

    SECTION("impossible") {
      auto invalid_vectors = vectors.filter(all_even)->filter(all_odd);
      REQUIRE_THROWS_AS((*invalid_vectors)(),
                        strategies::OutOfCycles);
    }
  }
}


static void check_booleans_vectors_strategy() {
  size_t min_size = constants::min_capacity;

  strategies::Booleans false_values(0.);
  strategies::Booleans true_values(1.);

  SECTION("multiple elements domain") {
    size_t max_size = constants::max_capacity;

    const std::shared_ptr<strategies::Integers<size_t>> &sizes =
        std::make_shared<strategies::Integers<size_t>>(min_size,
                                                       max_size);
    strategies::Vectors<bool> true_vectors(
        sizes,
        std::make_shared<strategies::Booleans>(true_values));
    strategies::Vectors<bool> false_vectors(
        sizes,
        std::make_shared<strategies::Booleans>(false_values));

    auto true_vector = true_vectors();
    auto false_vector = false_vectors();

    auto stays_in_range = in_range_checker<size_t>(min_size,
                                                   max_size);

    REQUIRE(stays_in_range(true_vector.size()));
    REQUIRE(stays_in_range(false_vector.size()));

    REQUIRE(std::all_of(true_vector.begin(),
                        true_vector.end(),
                        identity));
    REQUIRE(std::all_of(false_vector.begin(),
                        false_vector.end(),
                        negate));
  }

  SECTION("filtration") {
    auto all_true = [&](std::vector<bool> vector) -> bool {
      return std::all_of(vector.begin(),
                         vector.end(),
                         identity);
    };
    auto all_false = [&](std::vector<bool> vector) -> bool {
      return std::all_of(vector.begin(),
                         vector.end(),
                         negate);
    };

    size_t max_size = sufficient_capacity(1, 2, // true or false
                                          strategies::MAX_CYCLES);
    const std::shared_ptr<strategies::Integers<size_t>> &sizes =
        std::make_shared<strategies::Integers<size_t>>(min_size,
                                                       max_size);
    const std::shared_ptr<strategies::Booleans> &elements =
        std::make_shared<strategies::Booleans>();
    strategies::Vectors<bool> vectors(sizes,
                                      elements);

    SECTION("truthfulness") {
      auto all_true_vectors = vectors.filter(all_true);
      auto all_false_vectors = vectors.filter(all_false);

      auto all_true_vector = (*all_true_vectors)();
      auto all_false_vector = (*all_false_vectors)();

      auto stays_in_range = in_range_checker(min_size,
                                             max_size);

      REQUIRE(stays_in_range(all_true_vector.size()));
      REQUIRE(stays_in_range(all_false_vector.size()));
      REQUIRE(all_true(all_true_vector));
      REQUIRE(all_false(all_false_vector));
    }

    SECTION("impossible") {
      auto invalid_vectors = vectors.filter(all_true)->filter(all_false);
      REQUIRE_THROWS_AS((*invalid_vectors)(),
                        strategies::OutOfCycles);
    }
  }
}


static void check_characters_vectors_strategy() {
  std::string non_zero_characters = factories::non_zero_characters();

  SECTION("single character") {
    auto ones = std::make_shared<strategies::Just<size_t>>(1);
    for (char single_character: non_zero_characters) {
      std::string single_character_string{single_character};
      auto same_character = std::make_shared<strategies::Characters>(
          single_character_string);
      strategies::Vectors<char> same_character_vectors(ones,
                                                       same_character);

      auto same_character_vector = same_character_vectors();

      REQUIRE(std::all_of(same_character_vector.begin(),
                          same_character_vector.end(),
                          [=](char character) -> bool {
                            return character == single_character;
                          }));
    }
  }

  SECTION("multiple characters") {
    size_t min_size = 0;
    size_t max_size = constants::max_capacity;
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    std::string characters_string = factories::characters_string();
    auto characters = std::make_shared<strategies::Characters>(
        characters_string);
    strategies::Vectors<char> characters_vectors(sizes,
                                                 characters);

    auto characters_vector = characters_vectors();
    std::vector<char> characters_domain(characters_string.begin(),
                                        characters_string.end());

    REQUIRE(is_vector_from_domain(characters_vector,
                                  characters_domain));
  }
  SECTION("filtration") {
    size_t min_size = constants::min_capacity;
    size_t max_size = sufficient_capacity(
        constants::alphanumeric_characters_count,
        constants::non_zero_characters_count,
        strategies::MAX_CYCLES);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto all_lower = [&](const std::vector<char> vector) -> bool {
      return std::all_of(vector.begin(),
                         vector.end(),
                         is_lower);
    };
    auto all_upper = [&](const std::vector<char> vector) -> bool {
      return std::all_of(vector.begin(),
                         vector.end(),
                         is_upper);
    };

    auto non_zero = std::make_shared<strategies::Characters>(
        non_zero_characters);
    strategies::Vectors<char> non_zero_vectors(sizes,
                                               non_zero);

    SECTION("case") {
      auto all_lower_vectors = non_zero_vectors.filter(all_lower);
      auto all_upper_vectors = non_zero_vectors.filter(all_upper);

      auto all_lower_vector = (*all_lower_vectors)();
      auto all_upper_vector = (*all_upper_vectors)();

      REQUIRE(all_lower(all_lower_vector));
      REQUIRE(all_upper(all_upper_vector));
    }

    SECTION("impossible") {
      auto invalid_vectors =
          non_zero_vectors.filter(all_lower)->filter(all_upper);

      REQUIRE_THROWS_AS((*invalid_vectors)(),
                        strategies::OutOfCycles);
    }
  }
}


static void check_strings_vectors_strategy() {
  std::string non_zero_characters = factories::non_zero_characters();

  SECTION("single character alphabet") {
    auto ones = std::make_shared<strategies::Just<size_t>>(1);
    for (char single_character: non_zero_characters) {
      std::string single_character_string{single_character};
      auto same_character = std::make_shared<strategies::Characters>(
          single_character_string);
      auto same_character_strings = std::make_shared<strategies::Strings>(
          ones,
          same_character);
      strategies::Vectors<std::string> same_character_strings_vectors(
          ones,
          same_character_strings);

      auto vector = same_character_strings_vectors();
      std::vector<std::string>
          single_character_string_vector{single_character_string};

      REQUIRE(vector == single_character_string_vector);
    }
  }
  SECTION("multiple characters alphabet") {
    size_t min_size = 0;
    size_t max_size = constants::max_capacity;
    size_t min_length = 0;
    size_t max_length = constants::max_capacity;
    auto lengths = std::make_shared<strategies::Integers<size_t>>(min_length,
                                                                  max_length);

    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);

    std::string alphabet_characters = factories::characters_string(
        constants::min_capacity,
        constants::max_capacity);
    auto alphabet = std::make_shared<strategies::Characters>(
        alphabet_characters);
    auto strings = std::make_shared<strategies::Strings>(lengths,
                                                         alphabet);
    strategies::Vectors<std::string> strings_vectors(sizes,
                                                     strings);

    auto strings_vector = strings_vectors();
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    auto length_stays_in_range = in_range_checker<size_t>(min_length,
                                                          max_length);
    auto lengths_stay_in_range =
        [&](const std::vector<std::string> &vector) -> bool {
          std::all_of(vector.begin(),
                      vector.end(),
                      [&](const std::string &string) -> bool {
                        return length_stays_in_range(string.length());
                      });
        };

    REQUIRE(size_stays_in_range(strings_vector.size()));
    REQUIRE(lengths_stay_in_range(strings_vector));
    REQUIRE(std::all_of(strings_vector.begin(),
                        strings_vector.end(),
                        [&](const std::string &string) -> bool {
                          return is_string_from_alphabet(string,
                                                         alphabet_characters);
                        }));
  }
  SECTION("filtration") {
    auto is_digits_vector =
        [&](const std::vector<std::string> &vector) -> bool {
          return std::all_of(vector.begin(),
                             vector.end(),
                             is_digits_string);
        };
    auto is_alphabetic_vector =
        [&](const std::vector<std::string> &vector) -> bool {
          return std::all_of(vector.begin(),
                             vector.end(),
                             is_alphabetic_string);
        };

    /* if ``min_length``/``min_size`` equals to zero
     * than impossible would not raise exception
     * since it is possible to avoid filters with empty string/vector.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = min_size + 1;
    size_t min_length = constants::min_capacity;
    size_t max_length = sufficient_capacity(
        constants::alphanumeric_characters_count,
        constants::non_zero_characters_count,
        strategies::MAX_CYCLES / max_size);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto lengths = std::make_shared<strategies::Integers<size_t>>(min_length,
                                                                  max_length);

    strategies::Characters non_zero(non_zero_characters);
    auto alphanumeric_characters = non_zero.filter(is_alphanumeric);
    auto alphanumeric_strings = std::make_shared<strategies::Strings>(
        lengths,
        std::move(alphanumeric_characters));
    strategies::Vectors<std::string> alphanumeric(sizes,
                                                  alphanumeric_strings);

    SECTION("alphanumeric") {
      auto digits_vectors = alphanumeric.filter(is_digits_vector);
      auto alphabetic_vectors = alphanumeric.filter(is_alphabetic_vector);

      auto digits_vector = (*digits_vectors)();
      auto alphabetic_vector = (*alphabetic_vectors)();
      auto sizes_stays_in_range = in_range_checker<size_t>(min_size,
                                                           max_size);
      auto length_stays_in_range = in_range_checker<size_t>(min_length,
                                                            max_length);
      auto lengths_stay_in_range =
          [&](const std::vector<std::string> &vector) -> bool {
            std::all_of(vector.begin(),
                        vector.end(),
                        [&](const std::string &string) -> bool {
                          return length_stays_in_range(string.length());
                        });
          };

      REQUIRE(sizes_stays_in_range(digits_vector.size()));
      REQUIRE(sizes_stays_in_range(alphabetic_vector.size()));
      REQUIRE(lengths_stay_in_range(digits_vector));
      REQUIRE(lengths_stay_in_range(alphabetic_vector));
      REQUIRE(is_digits_vector(digits_vector));
      REQUIRE(is_alphabetic_vector(alphabetic_vector));
    }

    SECTION("impossible") {
      auto invalid_vectors =
          alphanumeric.filter(is_digits_vector)->filter(is_alphabetic_vector);
      REQUIRE_THROWS_AS((*invalid_vectors)(),
                        strategies::OutOfCycles);
    }
  }
}


TEST_CASE("\"vectors\" strategy", "[vectors]") {
  SECTION("bool") {
    check_booleans_vectors_strategy();
  }
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
  SECTION("characters") {
    check_characters_vectors_strategy();
  }
  SECTION("strings") {
    check_strings_vectors_strategy();
  }
}
