#include <catch.hpp>
#include <iostream>
#include "../cauldron/just.h"
#include "../cauldron/strings.h"
#include "predicates.h"


bool is_string_from_alphabet(const std::string &string,
                             const std::string &alphabet_characters) {
  auto is_character_from_alphabet = [=](char character) -> bool {
    return is_character_in_string(character, alphabet_characters);
  };
  return std::all_of(string.begin(),
                     string.end(),
                     is_character_from_alphabet);
}


TEST_CASE("\"strings\" strategy", "[strings]") {
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

  SECTION("single character alphabet") {
    strategies::Just<size_t> ones(1);
    for (char single_character: non_zero_ascii_characters) {
      std::string single_character_string{single_character};
      strategies::Characters same_character(single_character_string);
      strategies::Strings same_character_strings(
          std::make_shared<strategies::Just<size_t>>(ones),
          std::make_shared<strategies::Characters>(same_character));

      auto string = same_character_strings();

      REQUIRE(string == single_character_string);
    }
  }

  SECTION("multiple characters alphabet") {
    strategies::Integers<size_t> alphabets_lengths(min_length, max_length);
    strategies::Integers<size_t> strings_lengths(0, max_length);
    auto alphabet_length = alphabets_lengths();
    strategies::Integers<char> characters_integers;
    std::string alphabet_characters;
    for (size_t _ = 0; _ < alphabet_length; ++_) {
      alphabet_characters.push_back(characters_integers());
    }
    strategies::Characters alphabet(alphabet_characters);
    strategies::Strings strings(
        std::make_shared<strategies::Integers<size_t>>(strings_lengths),
        std::make_shared<strategies::Characters>(alphabet));

    auto string = strings();

    REQUIRE(min_length <= string.length() <= max_length);
    REQUIRE(is_string_from_alphabet(string,
                                    alphabet_characters));
  }

  SECTION("filtration") {
    auto all_digits = [&](const std::string &string) -> bool {
      return std::all_of(string.begin(),
                         string.end(),
                         is_digit);
    };
    auto all_alphabetic = [&](const std::string &string) -> bool {
      return std::all_of(string.begin(),
                         string.end(),
                         is_alphabetic);
    };
    size_t max_length = 3;
    strategies::Integers<size_t> strings_lengths(min_length,
                                                 max_length);
    strategies::Characters non_zero_ascii(non_zero_ascii_characters);
    auto alphanumeric = non_zero_ascii.filter(is_alphanumeric);
    strategies::Strings strings(
        std::make_shared<strategies::Integers<size_t>>(strings_lengths),
        std::move(alphanumeric));

    SECTION("alphanumeric") {
      auto all_digits_strings = strings.filter(all_digits);
      auto all_alphabetic_strings = strings.filter(all_alphabetic);

      auto all_digits_string = (*all_digits_strings)();
      auto all_alphabetic_string = (*all_alphabetic_strings)();

      REQUIRE(min_length <= all_digits_string.length() <= max_length);
      REQUIRE(all_digits(all_digits_string));
      REQUIRE(all_alphabetic(all_alphabetic_string));
    }

    SECTION("impossible") {
      auto invalid_strings =
          strings.filter(all_digits)->filter(all_alphabetic);
      REQUIRE_THROWS_AS((*invalid_strings)(),
                        strategies::OutOfCycles);
    }
  }
}
