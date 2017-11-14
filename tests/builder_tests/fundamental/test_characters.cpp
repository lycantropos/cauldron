#include <catch.hpp>
#include <cauldron/just.h>
#include <cauldron/characters.h>
#include <cauldron/builder.h>

#include <tests/factories.h>
#include <tests/predicates.h>
#include <tests/operators.h>
#include <tests/builder_tests/wrapper.h>


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
    for (char single_character: non_zero_characters) {
      std::string single_character_string{single_character};
      cauldron::Characters same_character(single_character_string);
      cauldron::Builder<CharacterWrapper, char> same_character_wrappers(
          same_character);

      auto wrapper = same_character_wrappers();

      REQUIRE(wrapper == CharacterWrapper(single_character));
    }
  }

  SECTION("multiple characters") {
    std::string characters_string = factories::characters_string();
    cauldron::Characters characters(characters_string);
    cauldron::Builder<CharacterWrapper, char> characters_wrappers(characters);
    std::set<char> characters_domain(characters_string.begin(),
                                     characters_string.end());

    auto wrapper = characters_wrappers();

    REQUIRE(is_object_in_set<char>(wrapper.field(),
                                   characters_domain));
  }

  SECTION("filtration") {
    auto alphabetic_characters =
        cauldron::Characters(non_zero_characters).filter(is_alphabetic);
    cauldron::Builder<CharacterWrapper, char> alphabetic(
        *alphabetic_characters);

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
        *alphabetic_characters);

    SECTION("case") {
      auto lower_wrappers = alphabetic.map(to_lower_wrapper);
      auto upper_wrappers = alphabetic.map(to_upper_wrapper);

      auto lower_wrapper = lower_wrappers();
      auto upper_wrapper = upper_wrappers();

      REQUIRE(is_lower_wrapper(lower_wrapper));
      REQUIRE(is_upper_wrapper(upper_wrapper));
    }

    SECTION("impossible") {
      auto invalid_lower_wrappers =
          alphabetic.map(to_upper_wrapper).filter(is_lower_wrapper);
      auto invalid_upper_wrappers =
          alphabetic.map(to_lower_wrapper).filter(is_upper_wrapper);

      REQUIRE_THROWS_AS((*invalid_lower_wrappers)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_upper_wrappers)(),
                        cauldron::OutOfCycles);
    }
  }
}
