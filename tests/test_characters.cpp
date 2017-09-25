#include <catch.hpp>
#include <iostream>
#include "../cauldron/characters.h"
#include "utils.h"


TEST_CASE("\"characters\" strategy", "[characters]") {
  SECTION("single character") {
    for (char single_character = 1;
         single_character < std::numeric_limits<signed char>::max();
         ++single_character) {
      auto single_character_string = std::string({single_character});
      strategies::Characters same_character(single_character_string);
      auto character = same_character();
      bool are_characters_equal = character == single_character;
      REQUIRE(are_characters_equal);
    }
  }
  SECTION("multiple characters") {
    strategies::Integers<unsigned long> lengths(1, 100);
    strategies::Integers<char> characters_integers;
    auto length = lengths();
    std::string characters_string;
    for (unsigned long _ = 0; _ < length; ++_) {
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
}
