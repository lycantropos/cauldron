#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/characters.h"
#include "../../../cauldron/builder.h"
#include "../../factories.h"
#include "../../predicates.h"
#include "../../operators.h"
#include "../wrapper.h"


TEST_CASE("characters \"Builder\" strategy", "[Builder]") {
  using CharacterWrapper = Wrapper<char>;

  cauldron::Requirement<CharacterWrapper> is_lower_wrapper(
      [&](const CharacterWrapper &wrapper) -> bool {
        return is_lower(wrapper.field());
      });
  cauldron::Requirement<CharacterWrapper> is_upper_wrapper(
      [&](const CharacterWrapper &wrapper) -> bool {
        return is_upper(wrapper.field());
      });

  std::string non_zero_characters = factories::non_zero_characters();

  SECTION("single character") {
    auto ones = std::make_shared<cauldron::Just<size_t>>(1);
    for (char single_character: non_zero_characters) {
      std::string single_character_string{single_character};
      auto same_character = std::make_shared<cauldron::Characters>(
          single_character_string);
      cauldron::Builder<CharacterWrapper, char> same_character_wrappers(
          same_character);

      auto wrapper = same_character_wrappers();

      REQUIRE(wrapper == CharacterWrapper(single_character));
    }
  }

  SECTION("multiple characters") {
    std::string characters_string = factories::characters_string();
    auto characters = std::make_shared<cauldron::Characters>(
        characters_string);
    cauldron::Builder<CharacterWrapper, char> characters_wrappers(characters);
    std::set<char> characters_domain(characters_string.begin(),
                                     characters_string.end());

    auto wrapper = characters_wrappers();

    REQUIRE(is_object_in_set<char>(wrapper.field(),
                                   characters_domain));
  }

  SECTION("union") {
    SECTION("single character") {
      auto ones = std::make_shared<cauldron::Just<size_t>>(1);
      for (char single_character: non_zero_characters) {
        std::string single_character_string{single_character};
        auto same_character = std::make_shared<cauldron::Characters>(
            single_character_string);
        cauldron::Builder<CharacterWrapper, char> same_character_wrappers(
            same_character);
        auto still_same_character_wrappers =
            same_character_wrappers || same_character_wrappers;

        auto wrapper = still_same_character_wrappers();

        REQUIRE(wrapper == CharacterWrapper(single_character));
      }
    }

    SECTION("multiple characters") {
      std::string characters_string = factories::characters_string();
      std::set<char> characters_domain(characters_string.begin(),
                                       characters_string.end());
      auto characters = std::make_shared<cauldron::Characters>(
          characters_string);
      cauldron::Builder<CharacterWrapper, char>
          characters_wrappers(characters);
      auto still_characters_wrappers =
          characters_wrappers || characters_wrappers;

      auto wrapper = still_characters_wrappers();

      REQUIRE(is_object_in_set<char>(wrapper.field(),
                                     characters_domain));
    }
  }

  SECTION("filtration") {
    auto alphabetic_characters =
        cauldron::Characters(non_zero_characters).filter(is_alphabetic);
    cauldron::Builder<CharacterWrapper, char> alphabetic(
        std::move(alphabetic_characters));

    SECTION("case") {
      auto lower_wrappers = alphabetic.filter(is_lower_wrapper);
      auto upper_wrappers = alphabetic.filter(is_upper_wrapper);

      auto lower_wrapper = (*lower_wrappers)();
      auto upper_wrapper = (*upper_wrappers)();

      REQUIRE(is_lower_wrapper(lower_wrapper));
      REQUIRE(is_upper_wrapper(upper_wrapper));
    }

    SECTION("impossible") {
      auto invalid_wrappers =
          alphabetic.filter(is_lower_wrapper)->filter(is_upper_wrapper);

      REQUIRE_THROWS_AS((*invalid_wrappers)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    cauldron::Converter<CharacterWrapper> to_lower_wrapper(
        [&](const CharacterWrapper &wrapper) -> CharacterWrapper {
          return CharacterWrapper(to_lower(wrapper.field()));
        });
    cauldron::Converter<CharacterWrapper> to_upper_wrapper(
        [&](const CharacterWrapper &wrapper) -> CharacterWrapper {
          return CharacterWrapper(to_upper(wrapper.field()));
        });

    auto alphabetic_characters =
        cauldron::Characters(non_zero_characters).filter(is_alphabetic);
    cauldron::Builder<CharacterWrapper, char> alphabetic(
        std::move(alphabetic_characters));

    SECTION("case") {
      auto lower_wrappers = alphabetic.map(to_lower_wrapper);
      auto upper_wrappers = alphabetic.map(to_upper_wrapper);

      auto lower_wrapper = (*lower_wrappers)();
      auto upper_wrapper = (*upper_wrappers)();

      REQUIRE(is_lower_wrapper(lower_wrapper));
      REQUIRE(is_upper_wrapper(upper_wrapper));
    }

    SECTION("impossible") {
      auto invalid_lower_wrappers =
          alphabetic.map(to_upper_wrapper)->filter(is_lower_wrapper);
      auto invalid_upper_wrappers =
          alphabetic.map(to_lower_wrapper)->filter(is_upper_wrapper);

      REQUIRE_THROWS_AS((*invalid_lower_wrappers)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_upper_wrappers)(),
                        cauldron::OutOfCycles);
    }
  }
}
