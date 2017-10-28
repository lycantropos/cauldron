#include <catch.hpp>
#include <iostream>
#include "../cauldron/characters.h"
#include "operators.h"
#include "predicates.h"
#include "factories.h"


TEST_CASE("\"characters\" strategy", "[characters]") {
  std::string non_zero_characters = factories::non_zero_characters();

  SECTION("single character") {
    for (char single_character: non_zero_characters) {
      std::string single_character_string{single_character};
      cauldron::Characters same_character(single_character_string);

      auto character = same_character();

      REQUIRE(character == single_character);
    }
  }

  SECTION("multiple characters") {
    std::string characters_string = factories::characters_string(
        constants::min_capacity,
        constants::max_capacity);
    cauldron::Characters characters(characters_string);

    auto character = characters();

    REQUIRE(is_character_in_string(character,
                                   characters_string));
  }

  SECTION("invalid whitelist characters") {
    REQUIRE_THROWS_AS(cauldron::Characters(""),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(cauldron::Characters("\0"),
                      std::invalid_argument);
  }

  SECTION("filtration") {
    cauldron::Characters non_zero(non_zero_characters);

    SECTION("case") {
      auto lower_characters = non_zero.filter(is_lower);
      auto upper_characters = non_zero.filter(is_upper);

      auto lower_character = (*lower_characters)();
      auto upper_character = (*upper_characters)();

      REQUIRE(is_lower(lower_character));
      REQUIRE(is_upper(upper_character));
    }

    SECTION("impossible") {
      auto invalid_characters = non_zero.filter(is_lower)->filter(is_upper);

      REQUIRE_THROWS_AS((*invalid_characters)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    cauldron::Characters non_zero(non_zero_characters);
    auto alphabetic = non_zero.filter(is_alphabetic);

    SECTION("case") {
      auto lower_characters = alphabetic->map(to_lower);
      auto upper_characters = alphabetic->map(to_upper);

      auto lower_character = (*lower_characters)();
      auto upper_character = (*upper_characters)();

      REQUIRE(is_lower(lower_character));
      REQUIRE(is_upper(upper_character));
    }

    SECTION("impossible") {
      auto invalid_characters = non_zero.map(to_upper)->filter(is_lower);

      REQUIRE_THROWS_AS((*invalid_characters)(),
                        cauldron::OutOfCycles);
    }
  }
}
