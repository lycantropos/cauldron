#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/characters.h"
#include "../../../cauldron/strings.h"
#include "../../../cauldron/builder.h"
#include "../../factories.h"
#include "../../predicates.h"
#include "../../operators.h"
#include "../wrapper.h"


TEST_CASE("strings \"Builder\" strategy", "[Builder]") {
  using StringWrapper = Wrapper<std::string>;

  cauldron::Requirement<StringWrapper> is_lower_wrapper(
      [&](const StringWrapper &wrapper) -> bool {
        return is_lower_string(wrapper.field());
      });
  cauldron::Requirement<StringWrapper> is_upper_wrapper(
      [&](const StringWrapper &wrapper) -> bool {
        return is_upper_string(wrapper.field());
      });

  size_t max_length = constants::max_capacity;

  std::string non_zero_characters = factories::non_zero_characters();

  SECTION("single character alphabet") {
    auto ones = std::make_shared<cauldron::Just<size_t>>(1);
    for (char single_character: non_zero_characters) {
      std::string single_character_string{single_character};
      auto same_character = std::make_shared<cauldron::Characters>(
          single_character_string);
      auto same_character_strings = std::make_shared<cauldron::Strings>(
          ones,
          same_character);
      cauldron::Builder<StringWrapper, std::string>
          same_character_strings_wrappers(same_character_strings);

      auto wrapper = same_character_strings_wrappers();

      REQUIRE(wrapper == StringWrapper(single_character_string));
    }
  }

  SECTION("multiple characters alphabet") {
    size_t min_length = 0;
    auto length_stays_in_range = in_range_checker<size_t>(min_length,
                                                          max_length);
    auto lengths = std::make_shared<cauldron::Integers<size_t>>(min_length,
                                                                max_length);
    std::string alphabet_characters = factories::characters_string(
        constants::min_capacity,
        constants::max_capacity);
    auto alphabet = std::make_shared<cauldron::Characters>(
        alphabet_characters);
    auto strings = std::make_shared<cauldron::Strings>(lengths,
                                                       alphabet);
    cauldron::Builder<StringWrapper, std::string> strings_wrappers(strings);

    auto wrapper = strings_wrappers();

    REQUIRE(length_stays_in_range(wrapper.field().length()));
    REQUIRE(is_string_from_alphabet(wrapper.field(),
                                    alphabet_characters));
  }

  SECTION("filtration") {
    /* if ``min_length``/``min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty string/wrapper.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = min_size + 1;
    size_t min_length = constants::min_capacity;
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    auto lengths = std::make_shared<cauldron::Integers<size_t>>(min_length,
                                                                max_length);

    auto alphabetic_characters =
        cauldron::Characters(non_zero_characters).filter(is_alphabetic);
    auto alphabetic_strings = std::make_shared<cauldron::Strings>(
        lengths,
        std::move(alphabetic_characters));
    cauldron::Builder<StringWrapper, std::string> alphabetic(
        alphabetic_strings);

    SECTION("case") {
      auto lower_wrappers = alphabetic.filter(is_lower_wrapper);
      auto upper_wrappers = alphabetic.filter(is_upper_wrapper);

      auto lower_wrapper = (*lower_wrappers)();
      auto upper_wrapper = (*upper_wrappers)();
      auto sizes_stays_in_range = in_range_checker<size_t>(min_size,
                                                           max_size);
      auto length_stays_in_range = in_range_checker<size_t>(min_length,
                                                            max_length);

      REQUIRE(length_stays_in_range(lower_wrapper.field().length()));
      REQUIRE(length_stays_in_range(upper_wrapper.field().length()));
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
    cauldron::Converter<StringWrapper> to_lower_wrapper(
        [&](const StringWrapper &wrapper) -> StringWrapper {
          return StringWrapper(to_lower_string(wrapper.field()));
        });
    cauldron::Converter<StringWrapper> to_upper_wrapper(
        [&](const StringWrapper &wrapper) -> StringWrapper {
          return StringWrapper(to_upper_string(wrapper.field()));
        });

    /* if ``min_length``/``min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty string/wrapper.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = min_size + 1;
    size_t min_length = constants::min_capacity;
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    auto lengths = std::make_shared<cauldron::Integers<size_t>>(min_length,
                                                                max_length);

    auto alphabetic_characters =
        cauldron::Characters(non_zero_characters).filter(is_alphabetic);
    auto alphabetic_strings = std::make_shared<cauldron::Strings>(
        lengths,
        std::move(alphabetic_characters));
    cauldron::Builder<StringWrapper, std::string> alphabetic(
        alphabetic_strings);

    SECTION("case") {
      auto lower_wrappers = alphabetic.map(to_lower_wrapper);
      auto upper_wrappers = alphabetic.map(to_upper_wrapper);

      auto lower_wrapper = (*lower_wrappers)();
      auto upper_wrapper = (*upper_wrappers)();
      auto sizes_stays_in_range = in_range_checker<size_t>(min_size,
                                                           max_size);
      auto length_stays_in_range = in_range_checker<size_t>(min_length,
                                                            max_length);

      REQUIRE(length_stays_in_range(lower_wrapper.field().length()));
      REQUIRE(length_stays_in_range(upper_wrapper.field().length()));
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
