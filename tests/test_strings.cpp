#include <catch.hpp>
#include <iostream>
#include "../cauldron/strings.h"
#include "utils.h"
#include "../cauldron/just.h"


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
  size_t min_length = 1;
  size_t max_length = 100;
  auto max_ascii_character = std::numeric_limits<char>::max();
  std::string non_zero_ascii_characters;
  for (char character = 1;
       character < max_ascii_character;
       ++character) {
    non_zero_ascii_characters.push_back(character);
  }

  SECTION("single character alphabet") {
    strategies::Just<size_t> ones(1);
    for (char single_character: non_zero_ascii_characters) {
      auto single_character_string = std::string({single_character});
      strategies::Characters same_character(single_character_string);
      strategies::Strings same_character_strings(&ones,
                                                 &same_character);

      auto string = same_character_strings();

      REQUIRE(string == single_character_string);
    }
  }

  SECTION("multiple characters alphabet") {
    strategies::Integers<size_t> alphabets_lengths(min_length, max_length);
    strategies::Integers<size_t> strings_lengths(0, max_length);
    auto alphabet_length = alphabets_lengths();
    std::string alphabet_characters;
    strategies::Integers<char> characters_integers;
    for (size_t _ = 0; _ < alphabet_length; ++_) {
      alphabet_characters.push_back(characters_integers());
    }
    strategies::Characters alphabet(alphabet_characters);
    strategies::Strings strings(&strings_lengths,
                                &alphabet);

    auto string = strings();

    REQUIRE(min_length <= string.length() <= max_length);
    REQUIRE(is_string_from_alphabet(string, alphabet_characters));
  }

  SECTION("filtration") {
    auto all_digits = [](std::string string) -> bool {
      return std::all_of(string.begin(),
                         string.end(),
                         is_digit);
    };
    auto all_alphabetic = [](std::string string) -> bool {
      return std::all_of(string.begin(),
                         string.end(),
                         is_alphabetic);
    };
    size_t max_length = 2;
    strategies::Integers<size_t> strings_lengths(min_length,
                                                 max_length);
    strategies::Characters non_zero_ascii(non_zero_ascii_characters);
    auto alphanumeric = non_zero_ascii.filter(is_alphanumeric);
    strategies::Strings strings(&strings_lengths,
                                &alphanumeric);

    SECTION("alphanumeric") {
      auto all_digits_strings = strings.filter(all_digits);
      auto all_alphabetic_strings = strings.filter(all_alphabetic);

      auto all_digits_string = all_digits_strings();
      auto all_alphabetic_string = all_alphabetic_strings();

      REQUIRE(min_length <= all_digits_string.length() <= max_length);
      REQUIRE(all_digits(all_digits_string));
    }

    SECTION("impossible") {
      auto invalid_strings = strings.filter(all_digits).filter(all_alphabetic);
      REQUIRE_THROWS_AS(invalid_strings(),
                        strategies::OutOfTries);
    }
  }
}
