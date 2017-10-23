#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/booleans.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/characters.h"
#include "../../../cauldron/sets.h"
#include "../../factories.h"
#include "../../predicates.h"
#include "../../operators.h"
#include "../../utils.h"


TEST_CASE("characters \"Sets\" strategy", "[Sets]") {
  strategies::Requirement<std::set<char>> is_lower_set(
      [&](const std::set<char> &set) -> bool {
        return std::all_of(set.begin(),
                           set.end(),
                           is_lower);
      });
  strategies::Requirement<std::set<char>> is_upper_set(
      [&](const std::set<char> &set) -> bool {
        return std::all_of(set.begin(),
                           set.end(),
                           is_upper);
      });

  std::string non_zero_characters = factories::non_zero_characters();

  SECTION("single character") {
    auto ones = std::make_shared<strategies::Just<size_t>>(1);
    for (char single_character: non_zero_characters) {
      std::string single_character_string{single_character};
      auto same_character = std::make_shared<strategies::Characters>(
          single_character_string);
      strategies::Sets<char> same_character_sets(ones,
                                                 same_character);

      auto same_character_set = same_character_sets();

      REQUIRE(std::all_of(same_character_set.begin(),
                          same_character_set.end(),
                          [=](char character) -> bool {
                            return character == single_character;
                          }));
    }
  }

  SECTION("multiple characters") {
    std::string characters_string = factories::characters_string();
    size_t min_size = 0;
    size_t max_size = sufficient_set_size(characters_string.length(),
                                          strategies::MAX_CYCLES);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto characters = std::make_shared<strategies::Characters>(
        characters_string);
    strategies::Sets<char> characters_sets(sizes,
                                           characters);
    std::set<char> characters_domain(characters_string.begin(),
                                     characters_string.end());

    auto characters_set = characters_sets();
    auto stays_in_range = in_range_checker<size_t>(min_size,
                                                   max_size);
    auto is_character_from_domain = [=](char character) -> bool {
      return is_object_in_set<char>(character, characters_domain);
    };
    auto is_set_from_characters_domain =
        [&](const std::set<char> &set) -> bool {
          return std::all_of(set.begin(),
                             set.end(),
                             is_character_from_domain);
        };

    REQUIRE(stays_in_range(characters_set.size()));
    REQUIRE(is_set_from_characters_domain(characters_set));
  }

  SECTION("filtration") {
    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = sufficient_set_size(
        constants::alphabetic_characters_count,
        strategies::MAX_CYCLES);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto alphabetic_characters =
        strategies::Characters(non_zero_characters).filter(is_alphabetic);
    strategies::Sets<char> alphabetic(sizes,
                                      std::move(alphabetic_characters));

    SECTION("case") {
      auto lower_sets = alphabetic.filter(is_lower_set);
      auto upper_sets = alphabetic.filter(is_upper_set);

      auto lower_set = (*lower_sets)();
      auto upper_set = (*upper_sets)();
      auto stays_in_range = in_range_checker<size_t>(min_size,
                                                     max_size);

      REQUIRE(stays_in_range(lower_set.size()));
      REQUIRE(stays_in_range(upper_set.size()));
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
    strategies::Converter<std::set<char>> to_lower_set(
        [&](const std::set<char> &set) -> std::set<char> {
          std::vector<char> vector;
          vector.reserve(set.size());
          std::transform(set.begin(),
                         set.end(),
                         std::back_inserter(vector),
                         to_lower);
          return std::set<char>(vector.begin(),
                                vector.end());
        });
    strategies::Converter<std::set<char>> to_upper_set(
        [&](const std::set<char> &set) -> std::set<char> {
          std::vector<char> vector;
          vector.reserve(set.size());
          std::transform(set.begin(),
                         set.end(),
                         std::back_inserter(vector),
                         to_upper);
          return std::set<char>(vector.begin(),
                                vector.end());
        });

    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = sufficient_set_size(
        constants::alphabetic_characters_count,
        strategies::MAX_CYCLES);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto alphabetic_characters =
        strategies::Characters(non_zero_characters).filter(is_alphabetic);
    strategies::Sets<char> alphabetic(sizes,
                                      std::move(alphabetic_characters));

    SECTION("case") {
      auto lower_sets = alphabetic.map(to_lower_set);
      auto upper_sets = alphabetic.map(to_upper_set);

      auto lower_set = (*lower_sets)();
      auto upper_set = (*upper_sets)();
      auto stays_in_range = in_range_checker<size_t>(min_size,
                                                     max_size);

      REQUIRE(stays_in_range(lower_set.size()));
      REQUIRE(stays_in_range(upper_set.size()));
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
