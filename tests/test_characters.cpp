#include <catch.hpp>
#include <iostream>
#include "../cauldron/characters.h"
#include "predicates.h"


TEST_CASE("\"characters\" strategy", "[characters]") {
  size_t min_length = 1;
  size_t max_length = 100;

  char min_character = std::numeric_limits<char>::min();
  char max_character = std::numeric_limits<char>::max();

  std::string non_zero_ascii_characters;
  for (char character = min_character;
       character < max_character;
       character = ++character + (character == 0)) {
    non_zero_ascii_characters.push_back(character);
  }

  SECTION("single character") {
    for (char single_character: non_zero_ascii_characters) {
      std::string single_character_string{single_character};
      strategies::Characters same_character(single_character_string);

      auto character = same_character();

      REQUIRE(character == single_character);
    }
  }

  SECTION("multiple characters") {
    strategies::Integers<size_t> lengths(min_length,
                                         max_length);
    strategies::Integers<char> characters_integers;
    auto length = lengths();
    std::string characters_string;
    for (size_t _ = 0; _ < length; ++_) {
      characters_string.push_back(characters_integers());
    }
    strategies::Characters characters(characters_string);

    auto character = characters();

    REQUIRE(is_character_in_string(character,
                                   characters_string));
  }

  SECTION("invalid whitelist characters") {
    REQUIRE_THROWS_AS(strategies::Characters(""),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(strategies::Characters("\0"),
                      std::invalid_argument);
  }

  SECTION("filtration") {
    strategies::Characters characters(non_zero_ascii_characters);

    SECTION("case") {
      auto lower_characters = characters.filter(is_lower);
      auto upper_characters = characters.filter(is_upper);

      auto lower_character = (*lower_characters)();
      auto upper_character = (*upper_characters)();

      REQUIRE(is_lower(lower_character));
      REQUIRE(is_upper(upper_character));
    }

    SECTION("impossible") {
      auto invalid_characters = characters.filter(is_lower)->filter(is_upper);

      REQUIRE_THROWS_AS((*invalid_characters)(),
                        strategies::OutOfCycles);
    }
  }
}
