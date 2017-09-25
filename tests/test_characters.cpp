#include <catch.hpp>
#include <iostream>
#include "../cauldron/characters.h"


bool is_character_in_string(const char &character,
                            const std::string &string) {
  return string.find(character) != -1;
}


TEST_CASE("\"characters\" strategy", "[characters]") {
  SECTION("single character") {
    for (char single_character = 1;
         single_character < std::numeric_limits<signed char>::max();
         ++single_character) {
      const std::string single_character_string = std::string({single_character});
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
    for (unsigned long i = 0; i < length; ++i) {
      characters_string.push_back(characters_integers());
    }
    strategies::Characters characters(characters_string);
    auto character = characters();
    REQUIRE(is_character_in_string(character, characters_string));
  }
}
