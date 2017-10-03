#include <catch.hpp>
#include <iostream>
#include "../cauldron/just.h"
#include "../cauldron/booleans.h"
#include "../cauldron/integers.h"
#include "../cauldron/characters.h"
#include "../cauldron/strings.h"
#include "../cauldron/vectors.h"
#include "factories.h"
#include "predicates.h"
#include "utils.h"


static void check_booleans_vectors_strategy() {
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
    REQUIRE(std::all_of(true_vector.begin(),
                        true_vector.end(),
                        identity));
    REQUIRE(std::all_of(false_vector.begin(),
                        false_vector.end(),
                        negate));
  }

  SECTION("filtration") {
    auto is_true_vector = [&](std::vector<bool> vector) -> bool {
      return std::all_of(vector.begin(),
                         vector.end(),
                         identity);
    };
    auto is_false_vector = [&](std::vector<bool> vector) -> bool {
      return std::all_of(vector.begin(),
                         vector.end(),
                         negate);
    };

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
}


template<typename T>
static void check_integers_vectors_strategy() {
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
    std::vector<char> characters_domain(characters_string.begin(),
                                        characters_string.end());

    auto characters_vector = characters_vectors();
    auto stays_in_range = in_range_checker<size_t>(min_size,
                                                   max_size);
    auto is_character_from_domain = [=](char character) -> bool {
      return is_object_in_vector<char>(character, characters_domain);
    };
    auto is_vector_from_characters_domain =
        [&](const std::vector<char> &vector) -> bool {
          return std::all_of(vector.begin(),
                             vector.end(),
                             is_character_from_domain);
        };

    REQUIRE(stays_in_range(characters_vector.size()));
    REQUIRE(is_vector_from_characters_domain(characters_vector));
  }
  SECTION("filtration") {
    size_t min_size = constants::min_capacity;
    size_t max_size = sufficient_capacity(
        constants::alphanumeric_characters_count,
        constants::non_zero_characters_count,
        strategies::MAX_CYCLES);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto is_lower_vector = [&](const std::vector<char> vector) -> bool {
      return std::all_of(vector.begin(),
                         vector.end(),
                         is_lower);
    };
    auto is_upper_vector = [&](const std::vector<char> vector) -> bool {
      return std::all_of(vector.begin(),
                         vector.end(),
                         is_upper);
    };

    auto non_zero = std::make_shared<strategies::Characters>(
        non_zero_characters);
    strategies::Vectors<char> non_zero_vectors(sizes,
                                               non_zero);

    SECTION("case") {
      auto lower_vectors = non_zero_vectors.filter(is_lower_vector);
      auto upper_vectors = non_zero_vectors.filter(is_upper_vector);

      auto lower_vector = (*lower_vectors)();
      auto upper_vector = (*upper_vectors)();
      auto stays_in_range = in_range_checker<size_t>(min_size,
                                                     max_size);

      REQUIRE(stays_in_range(lower_vector.size()));
      REQUIRE(stays_in_range(upper_vector.size()));
      REQUIRE(is_lower_vector(lower_vector));
      REQUIRE(is_upper_vector(upper_vector));
    }

    SECTION("impossible") {
      auto invalid_vectors =
          non_zero_vectors.filter(is_lower_vector)->filter(is_upper_vector);

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
     * than "impossible" section would not raise exception
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
