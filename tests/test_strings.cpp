#include <catch.hpp>
#include <iostream>
#include "../cauldron/strings.h"
#include "utils.h"


bool is_string_from_alphabet(const std::string &string,
                             const std::string &alphabet_characters) {
  for (auto character: string) {
    if (not is_character_in_string(character, alphabet_characters)) {
      return false;
    }
  }
  return true;
}


TEST_CASE("\"strings\" strategy", "[strings]") {
  SECTION("single character alphabet") {
    for (char single_character = 1;
         single_character < std::numeric_limits<signed char>::max();
         ++single_character) {
      auto single_character_string = std::string({single_character});
      strategies::Characters same_character(single_character_string);
      strategies::Strings same_character_strings(1, same_character);

      auto string = same_character_strings();

      REQUIRE(string == single_character_string);
    }
  }

  SECTION("multiple characters alphabet") {
    strategies::Integers<unsigned long> alphabets_lengths(1, 100);
    strategies::Integers<unsigned long> strings_lengths(0, 100);
    auto alphabet_length = alphabets_lengths();
    auto strings_length = alphabets_lengths();
    std::string alphabet_characters;
    strategies::Integers<char> characters_integers;
    for (unsigned long _ = 0; _ < alphabet_length; ++_) {
      alphabet_characters.push_back(characters_integers());
    }
    strategies::Characters alphabet(alphabet_characters);
    strategies::Strings strings(strings_length, alphabet);

    auto string = strings();

    REQUIRE(string.length() == strings_length);
    REQUIRE(is_string_from_alphabet(string, alphabet_characters));
  }
}
