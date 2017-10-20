#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/characters.h"
#include "../../../cauldron/strings.h"
#include "../../../cauldron/sets.h"
#include "../../factories.h"
#include "../../predicates.h"
#include "../../operators.h"

TEST_CASE("strings \"Sets\" strategy", "[Sets]") {
  auto is_lower_set =
      [&](const std::set<std::string> &set) -> bool {
        return std::all_of(set.begin(),
                           set.end(),
                           is_lower_string);
      };
  auto is_upper_set =
      [&](const std::set<std::string> &set) -> bool {
        return std::all_of(set.begin(),
                           set.end(),
                           is_upper_string);
      };

  size_t max_length = constants::max_capacity;

  std::string non_zero_characters = factories::non_zero_characters();

  SECTION("single character alphabet") {
    auto ones = std::make_shared<strategies::Just<size_t>>(1);
    for (char single_character: non_zero_characters) {
      std::string single_character_string{single_character};
      auto same_character = std::make_shared<strategies::Characters>(
          single_character_string);
      auto same_character_strings = std::make_shared<strategies::Strings>(
          ones,
          same_character);
      strategies::Sets<std::string> same_character_strings_sets(
          ones,
          same_character_strings);

      auto set = same_character_strings_sets();
      std::set<std::string>
          single_character_string_set{single_character_string};

      REQUIRE(set == single_character_string_set);
    }
  }

  SECTION("multiple characters alphabet") {
    size_t min_size = 0;
    size_t max_size = constants::max_capacity;
    size_t min_length = 0;
    auto lengths = std::make_shared<strategies::Integers<size_t>>(min_length,
                                                                  max_length);

    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);

    std::string alphabet_characters = factories::characters_string(
        constants::min_capacity,
        constants::max_capacity);
    auto alphabet = std::make_shared<strategies::Characters>(
        alphabet_characters);
    auto strings = std::make_shared<strategies::Strings>(lengths,
                                                         alphabet);
    strategies::Sets<std::string> strings_sets(sizes,
                                               strings);

    auto strings_set = strings_sets();
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    auto length_stays_in_range = in_range_checker<size_t>(min_length,
                                                          max_length);
    auto lengths_stay_in_range =
        [&](const std::set<std::string> &set) -> bool {
          return std::all_of(set.begin(),
                             set.end(),
                             [&](const std::string &string) -> bool {
                               return length_stays_in_range(string.length());
                             });
        };

    REQUIRE(size_stays_in_range(strings_set.size()));
    REQUIRE(lengths_stay_in_range(strings_set));
    REQUIRE(std::all_of(strings_set.begin(),
                        strings_set.end(),
                        [&](const std::string &string) -> bool {
                          return is_string_from_alphabet(string,
                                                         alphabet_characters);
                        }));
  }

  SECTION("filtration") {
    /* if ``min_length``/``min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty string/set.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = min_size + 1;
    size_t min_length = constants::min_capacity;
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto lengths = std::make_shared<strategies::Integers<size_t>>(min_length,
                                                                  max_length);

    auto alphabetic_characters =
        strategies::Characters(non_zero_characters).filter(is_alphabetic);
    auto alphabetic_strings = std::make_shared<strategies::Strings>(
        lengths,
        std::move(alphabetic_characters));
    strategies::Sets<std::string> alphabetic(sizes,
                                             alphabetic_strings);

    SECTION("case") {
      auto lower_sets = alphabetic.filter(is_lower_set);
      auto upper_sets = alphabetic.filter(is_upper_set);

      auto lower_set = (*lower_sets)();
      auto upper_set = (*upper_sets)();
      auto sizes_stays_in_range = in_range_checker<size_t>(min_size,
                                                           max_size);
      auto length_stays_in_range = in_range_checker<size_t>(min_length,
                                                            max_length);
      auto lengths_stay_in_range =
          [&](const std::set<std::string> &set) -> bool {
            return std::all_of(set.begin(),
                               set.end(),
                               [&](const std::string &string) -> bool {
                                 return length_stays_in_range(string.length());
                               });
          };

      REQUIRE(sizes_stays_in_range(lower_set.size()));
      REQUIRE(sizes_stays_in_range(upper_set.size()));
      REQUIRE(lengths_stay_in_range(lower_set));
      REQUIRE(lengths_stay_in_range(upper_set));
      REQUIRE(is_lower_set(lower_set));
      REQUIRE(is_upper_set(upper_set));
    }

    SECTION("impossible") {
      auto invalid_sets =
          alphabetic.filter(is_lower_set)->filter(is_upper_set);

      REQUIRE_THROWS_AS((*invalid_sets)(),
                        strategies::OutOfCycles);
    }
  }

  SECTION("mapping") {
    strategies::Converter<std::set<std::string>> to_lower_set(
        [&](const std::set<std::string> &set) -> std::set<std::string> {
          std::vector<std::string> vector;
          vector.reserve(set.size());
          std::transform(set.begin(),
                         set.end(),
                         std::back_inserter(vector),
                         to_lower_string);
          return std::set<std::string>(vector.begin(),
                                       vector.end());
        });
    strategies::Converter<std::set<std::string>> to_upper_set(
        [&](const std::set<std::string> &set) -> std::set<std::string> {
          std::vector<std::string> vector;
          vector.reserve(set.size());
          std::transform(set.begin(),
                         set.end(),
                         std::back_inserter(vector),
                         to_upper_string);
          return std::set<std::string>(vector.begin(),
                                       vector.end());
        });

    /* if ``min_length``/``min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty string/set.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = min_size + 1;
    size_t min_length = constants::min_capacity;
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto lengths = std::make_shared<strategies::Integers<size_t>>(min_length,
                                                                  max_length);

    auto alphabetic_characters =
        strategies::Characters(non_zero_characters).filter(is_alphabetic);
    auto alphabetic_strings = std::make_shared<strategies::Strings>(
        lengths,
        std::move(alphabetic_characters));
    strategies::Sets<std::string> alphabetic(sizes,
                                             alphabetic_strings);

    SECTION("case") {
      auto lower_sets = alphabetic.map(to_lower_set);
      auto upper_sets = alphabetic.map(to_upper_set);

      auto lower_set = (*lower_sets)();
      auto upper_set = (*upper_sets)();
      auto sizes_stays_in_range = in_range_checker<size_t>(min_size,
                                                           max_size);
      auto length_stays_in_range = in_range_checker<size_t>(min_length,
                                                            max_length);
      auto lengths_stay_in_range =
          [&](const std::set<std::string> &set) -> bool {
            return std::all_of(set.begin(),
                               set.end(),
                               [&](const std::string &string) -> bool {
                                 return length_stays_in_range(string.length());
                               });
          };

      REQUIRE(sizes_stays_in_range(lower_set.size()));
      REQUIRE(sizes_stays_in_range(upper_set.size()));
      REQUIRE(lengths_stay_in_range(lower_set));
      REQUIRE(lengths_stay_in_range(upper_set));
      REQUIRE(is_lower_set(lower_set));
      REQUIRE(is_upper_set(upper_set));
    }

    SECTION("impossible") {
      auto invalid_lower_sets =
          alphabetic.map(to_upper_set)->filter(is_lower_set);
      auto invalid_upper_sets =
          alphabetic.map(to_lower_set)->filter(is_upper_set);

      REQUIRE_THROWS_AS((*invalid_lower_sets)(),
                        strategies::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_upper_sets)(),
                        strategies::OutOfCycles);
    }
  }
}
