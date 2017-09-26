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

      REQUIRE(character == single_character);
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

  SECTION("filtration") {
    auto lower = [](char character) {
      return std::islower(character) != 0;
    };
    auto upper = [](char character) {
      return std::isupper(character) != 0;
    };
    strategies::Integers<unsigned long> lengths(1, 100);
    strategies::Integers<char> characters_integers;
    auto lower_characters_integers = characters_integers.filter(lower);
    auto upper_characters_integers = characters_integers.filter(upper);
    auto length = lengths();
    std::string characters_string;
    for (unsigned long _ = 0; _ < length; ++_) {
      characters_string.push_back(lower_characters_integers());
      characters_string.push_back(upper_characters_integers());
    }
    strategies::Characters characters(characters_string);

    SECTION("case") {
      auto lower_characters = characters.filter(lower);
      auto upper_characters = characters.filter(upper);

      auto lower_character = lower_characters();
      auto upper_character = upper_characters();

      REQUIRE(lower(lower_character));
      REQUIRE(upper(upper_character));
    }

    SECTION("impossible") {
      auto non_existent_characters = characters.filter(lower).filter(upper);

      REQUIRE_THROWS_AS(non_existent_characters(), strategies::OutOfTries);
    }
  }
}
