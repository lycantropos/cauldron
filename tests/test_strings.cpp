#include <catch.hpp>
#include <iostream>
#include "../cauldron/just.h"
#include "../cauldron/strings.h"
#include "predicates.h"
#include "factories.h"
#include "utils.h"


TEST_CASE("\"strings\" strategy", "[strings]") {
  std::string non_zero_characters = factories::non_zero_characters();

  SECTION("single character alphabet") {
    auto ones = std::make_shared<strategies::Just<size_t>>(1);
    for (char single_character: non_zero_characters) {
      std::string single_character_string{single_character};
      auto same_character = std::make_shared<strategies::Characters>(
          single_character_string);
      strategies::Strings same_character_strings(ones,
                                                 same_character);

      auto string = same_character_strings();

      REQUIRE(string == single_character_string);
    }
  }

  SECTION("multiple characters alphabet") {
    size_t min_length = 0;
    size_t max_length = constants::max_capacity;

    auto lengths = std::make_shared<strategies::Integers<size_t>>(min_length,
                                                                  max_length);
    std::string alphabet_characters = factories::characters_string();
    auto alphabet = std::make_shared<strategies::Characters>(
        alphabet_characters);
    strategies::Strings strings(lengths,
                                alphabet);

    auto string = strings();
    auto stays_in_range = in_range_checker<size_t>(min_length,
                                                   max_length);

    REQUIRE(stays_in_range(string.length()));
    REQUIRE(is_string_from_alphabet(string,
                                    alphabet_characters));
  }

  SECTION("filtration") {
    /* if ``min_length`` equals to zero
     * than impossible would not raise exception
     * since it is possible to avoid filters with empty string.
     */
    size_t min_length = constants::min_capacity;
    size_t max_length = sufficient_capacity(
        constants::alphanumeric_characters_count,
        constants::non_zero_characters_count,
        strategies::MAX_CYCLES);
    auto lengths = std::make_shared<strategies::Integers<size_t>>(min_length,
                                                                  max_length);

    strategies::Characters non_zero(non_zero_characters);
    auto alphanumeric_characters = non_zero.filter(is_alphanumeric);
    strategies::Strings alphanumeric(lengths,
                                     std::move(alphanumeric_characters));

    SECTION("alphanumeric") {
      auto digits_strings = alphanumeric.filter(is_digits_string);
      auto alphabetic_strings = alphanumeric.filter(is_alphabetic_string);

      auto digits_string = (*digits_strings)();
      auto alphabetic_string = (*alphabetic_strings)();
      auto stays_in_range = in_range_checker(min_length,
                                             max_length);

      REQUIRE(stays_in_range(digits_string.length()));
      REQUIRE(stays_in_range(alphabetic_string.length()));
      REQUIRE(is_digits_string(digits_string));
      REQUIRE(is_alphabetic_string(alphabetic_string));
    }

    SECTION("impossible") {
      auto invalid_strings =
          alphanumeric.filter(is_digits_string)->filter(is_alphabetic_string);
      REQUIRE_THROWS_AS((*invalid_strings)(),
                        strategies::OutOfCycles);
    }
  }
}
