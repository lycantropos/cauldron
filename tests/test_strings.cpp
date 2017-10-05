#include <catch.hpp>
#include <iostream>
#include "../cauldron/just.h"
#include "../cauldron/strings.h"
#include "predicates.h"
#include "factories.h"
#include "operators.h"


TEST_CASE("\"strings\" strategy", "[strings]") {
  std::string non_zero_characters_string = factories::non_zero_characters();
  size_t max_length = constants::max_capacity;

  SECTION("single character alphabet") {
    auto ones = std::make_shared<strategies::Just<size_t>>(1);
    for (char single_character: non_zero_characters_string) {
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
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty string.
     */
    size_t min_length = constants::min_capacity;
    auto lengths = std::make_shared<strategies::Integers<size_t>>(min_length,
                                                                  max_length);
    strategies::Characters non_zero_characters(non_zero_characters_string);
    auto alphabetic_characters = non_zero_characters.filter(is_alphabetic);
    strategies::Strings alphabetic(lengths,
                                   std::move(alphabetic_characters));

    SECTION("case") {
      auto lower_strings = alphabetic.filter(is_lower_string);
      auto upper_strings = alphabetic.filter(is_upper_string);

      auto lower_string = (*lower_strings)();
      auto upper_string = (*upper_strings)();
      auto stays_in_range = in_range_checker<size_t>(min_length,
                                                     max_length);

      REQUIRE(stays_in_range(lower_string.length()));
      REQUIRE(stays_in_range(upper_string.length()));
      REQUIRE(is_lower_string(lower_string));
      REQUIRE(is_upper_string(upper_string));
    }

    SECTION("impossible") {
      auto invalid_strings =
          alphabetic.filter(is_lower_string)->filter(is_upper_string);

      REQUIRE_THROWS_AS((*invalid_strings)(),
                        strategies::OutOfCycles);
    }
  }

  SECTION("mapping") {
    /* if ``min_length`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty string.
     */
    size_t min_length = constants::min_capacity;
    auto lengths = std::make_shared<strategies::Integers<size_t>>(min_length,
                                                                  max_length);
    strategies::Characters non_zero_characters(non_zero_characters_string);
    auto alphabetic_characters = non_zero_characters.filter(is_alphabetic);
    strategies::Strings alphabetic(lengths,
                                   std::move(alphabetic_characters));

    SECTION("case") {
      auto lower_strings = alphabetic.map(to_lower_string);
      auto upper_strings = alphabetic.map(to_upper_string);

      auto lower_string = (*lower_strings)();
      auto upper_string = (*upper_strings)();
      auto stays_in_range = in_range_checker<size_t>(min_length,
                                                     max_length);

      REQUIRE(stays_in_range(lower_string.length()));
      REQUIRE(stays_in_range(upper_string.length()));
      REQUIRE(is_lower_string(lower_string));
      REQUIRE(is_upper_string(upper_string));
    }

    SECTION("impossible") {
      auto invalid_lower_strings =
          alphabetic.map(to_upper_string)->filter(is_lower_string);
      auto invalid_upper_strings =
          alphabetic.map(to_lower_string)->filter(is_upper_string);

      REQUIRE_THROWS_AS((*invalid_lower_strings)(),
                        strategies::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_upper_strings)(),
                        strategies::OutOfCycles);
    }
  }
}
