#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/booleans.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/characters.h"
#include "../../../cauldron/strings.h"
#include "../../../cauldron/vectors.h"
#include "../../factories.h"
#include "../../predicates.h"
#include "../../operators.h"


TEST_CASE("characters \"Vectors\" strategy", "[Vectors]") {
  cauldron::Requirement<std::vector<char>> is_lower_vector(
      [&](const std::vector<char> vector) -> bool {
        return std::all_of(vector.begin(),
                           vector.end(),
                           is_lower);
      });
  cauldron::Requirement<std::vector<char>> is_upper_vector(
      [&](const std::vector<char> vector) -> bool {
        return std::all_of(vector.begin(),
                           vector.end(),
                           is_upper);
      });

  std::string non_zero_characters = factories::non_zero_characters();

  SECTION("single character") {
    auto ones = std::make_shared<cauldron::Just<size_t>>(1);
    for (char single_character: non_zero_characters) {
      std::string single_character_string{single_character};
      auto same_character = std::make_shared<cauldron::Characters>(
          single_character_string);
      cauldron::Vectors<char> same_character_vectors(ones,
                                                     same_character);

      auto same_character_vector = same_character_vectors();

      REQUIRE(std::all_of(same_character_vector.begin(),
                          same_character_vector.end(),
                          [=](char character) -> bool {
                            return character == single_character;
                          }));
    }
  }

  SECTION("multiple characters") {
    size_t min_size = 0;
    size_t max_size = constants::max_capacity;
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    std::string characters_string = factories::characters_string();
    auto characters = std::make_shared<cauldron::Characters>(
        characters_string);
    cauldron::Vectors<char> characters_vectors(sizes,
                                               characters);
    std::vector<char> characters_domain(characters_string.begin(),
                                        characters_string.end());

    auto characters_vector = characters_vectors();
    auto stays_in_range = in_range_checker<size_t>(min_size,
                                                   max_size);
    auto is_character_from_domain = [=](char character) -> bool {
      return is_object_in_vector<char>(character, characters_domain);
    };
    auto is_vector_from_characters_domain =
        [&](const std::vector<char> &vector) -> bool {
          return std::all_of(vector.begin(),
                             vector.end(),
                             is_character_from_domain);
        };

    REQUIRE(stays_in_range(characters_vector.size()));
    REQUIRE(is_vector_from_characters_domain(characters_vector));
  }

  SECTION("filtration") {
    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty vector.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = constants::max_capacity;
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    auto alphabetic_characters =
        cauldron::Characters(non_zero_characters).filter(is_alphabetic);
    cauldron::Vectors<char> alphabetic(sizes,
                                       std::move(alphabetic_characters));

    SECTION("case") {
      auto lower_vectors = alphabetic.filter(is_lower_vector);
      auto upper_vectors = alphabetic.filter(is_upper_vector);

      auto lower_vector = (*lower_vectors)();
      auto upper_vector = (*upper_vectors)();
      auto stays_in_range = in_range_checker<size_t>(min_size,
                                                     max_size);

      REQUIRE(stays_in_range(lower_vector.size()));
      REQUIRE(stays_in_range(upper_vector.size()));
      REQUIRE(is_lower_vector(lower_vector));
      REQUIRE(is_upper_vector(upper_vector));
    }

    SECTION("impossible") {
      auto invalid_vectors =
          alphabetic.filter(is_lower_vector)->filter(is_upper_vector);

      REQUIRE_THROWS_AS((*invalid_vectors)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    cauldron::Converter<std::vector<char>> to_lower_vector(
        [&](const std::vector<char> &vector) -> std::vector<char> {
          auto result = std::vector<char>(vector.size());
          std::transform(vector.begin(),
                         vector.end(),
                         result.begin(),
                         to_lower);
          return result;
        });
    cauldron::Converter<std::vector<char>> to_upper_vector(
        [&](const std::vector<char> &vector) -> std::vector<char> {
          auto result = std::vector<char>(vector.size());
          std::transform(vector.begin(),
                         vector.end(),
                         result.begin(),
                         to_upper);
          return result;
        });

    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty vector.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = constants::max_capacity;
    auto sizes = std::make_shared<cauldron::Integers<size_t>>(min_size,
                                                              max_size);
    auto alphabetic_characters =
        cauldron::Characters(non_zero_characters).filter(is_alphabetic);
    cauldron::Vectors<char> alphabetic(sizes,
                                       std::move(alphabetic_characters));

    SECTION("case") {
      auto lower_vectors = alphabetic.map(to_lower_vector);
      auto upper_vectors = alphabetic.map(to_upper_vector);

      auto lower_vector = (*lower_vectors)();
      auto upper_vector = (*upper_vectors)();
      auto stays_in_range = in_range_checker<size_t>(min_size,
                                                     max_size);

      REQUIRE(stays_in_range(lower_vector.size()));
      REQUIRE(stays_in_range(upper_vector.size()));
      REQUIRE(is_lower_vector(lower_vector));
      REQUIRE(is_upper_vector(upper_vector));
    }

    SECTION("impossible") {
      auto invalid_lower_vectors =
          alphabetic.map(to_upper_vector)->filter(is_lower_vector);
      auto invalid_upper_vectors =
          alphabetic.map(to_lower_vector)->filter(is_upper_vector);

      REQUIRE_THROWS_AS((*invalid_lower_vectors)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_upper_vectors)(),
                        cauldron::OutOfCycles);
    }
  }
}
