#include <catch.hpp>
#include <iostream>
#include "../cauldron/just.h"
#include "../cauldron/strings.h"
#include "predicates.h"
#include "factories.h"
#include "utils.h"


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
  std::string non_zero_characters = factories::non_zero_characters();

  SECTION("single character alphabet") {
    strategies::Just<size_t> ones(1);
    for (char single_character: non_zero_characters) {
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
    size_t min_length = 0;
    size_t max_length = constants::max_capacity;

    const std::shared_ptr<strategies::Integers<size_t>> &lengths =
        std::make_shared<strategies::Integers<size_t>>(min_length,
                                                       max_length);
    std::string alphabet_characters = factories::characters_string(
        constants::min_capacity,
        constants::max_capacity);
    const std::shared_ptr<strategies::Characters> &alphabet =
        std::make_shared<strategies::Characters>(alphabet_characters);
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

    size_t min_length = constants::min_capacity;
    size_t max_length = sufficient_capacity(
        constants::alphanumeric_characters_count,
        constants::non_zero_characters_count,
        strategies::MAX_CYCLES);

    strategies::Integers<size_t> strings_lengths(min_length,
                                                 max_length);
    strategies::Characters non_zero(non_zero_characters);
    auto alphanumeric = non_zero.filter(is_alphanumeric);
    strategies::Strings strings(
        std::make_shared<strategies::Integers<size_t>>(strings_lengths),
        std::move(alphanumeric));

    SECTION("alphanumeric") {
      auto all_digits_strings = strings.filter(all_digits);
      auto all_alphabetic_strings = strings.filter(all_alphabetic);

      auto all_digits_string = (*all_digits_strings)();
      auto all_alphabetic_string = (*all_alphabetic_strings)();
      auto stays_in_range = in_range_checker(min_length,
                                             max_length);

      REQUIRE(stays_in_range(all_digits_string.length()));
      REQUIRE(stays_in_range(all_alphabetic_string.length()));
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
