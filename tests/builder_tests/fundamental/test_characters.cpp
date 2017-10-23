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

  auto is_lower_wrapper = [&](const CharacterWrapper &wrapper) -> bool {
    return is_lower(wrapper.field());
  };
  auto is_upper_wrapper = [&](const CharacterWrapper &wrapper) -> bool {
    return is_upper(wrapper.field());
  };

  std::string non_zero_characters = factories::non_zero_characters();

  SECTION("single character") {
    auto ones = std::make_shared<strategies::Just<size_t>>(1);
    for (char single_character: non_zero_characters) {
      std::string single_character_string{single_character};
      auto same_character = std::make_shared<strategies::Characters>(
          single_character_string);
      strategies::Builder<CharacterWrapper, char> same_character_wrappers(
          same_character);

      auto same_character_wrapper = same_character_wrappers();

      bool character_is_the_same =
          same_character_wrapper.field() == single_character;
      REQUIRE(character_is_the_same);
    }
  }

  SECTION("multiple characters") {
    std::string characters_string = factories::characters_string();
    auto characters = std::make_shared<strategies::Characters>(
        characters_string);
    strategies::Builder<CharacterWrapper, char>
        characters_wrappers(characters);
    std::set<char> characters_domain(characters_string.begin(),
                                     characters_string.end());

    auto characters_wrapper = characters_wrappers();

    REQUIRE(is_object_in_set<char>(characters_wrapper.field(),
                                   characters_domain));
  }

  SECTION("filtration") {
    auto alphabetic_characters =
        strategies::Characters(non_zero_characters).filter(is_alphabetic);
    strategies::Builder<CharacterWrapper, char> alphabetic(
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
                        strategies::OutOfCycles);
    }
  }

  SECTION("mapping") {
    strategies::Converter<CharacterWrapper> to_lower_wrapper(
        [&](const CharacterWrapper &wrapper) -> CharacterWrapper {
          return CharacterWrapper(to_lower(wrapper.field()));
        });
    strategies::Converter<CharacterWrapper> to_upper_wrapper(
        [&](const CharacterWrapper &wrapper) -> CharacterWrapper {
          return CharacterWrapper(to_upper(wrapper.field()));
        });

    auto alphabetic_characters =
        strategies::Characters(non_zero_characters).filter(is_alphabetic);
    strategies::Builder<CharacterWrapper, char> alphabetic(
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
                        strategies::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_upper_wrappers)(),
                        strategies::OutOfCycles);
    }
  }
}
