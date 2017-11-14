#include <algorithm>

#include <catch.hpp>
#include <cauldron/just.h>
#include <cauldron/booleans.h>
#include <cauldron/integers.h>
#include <cauldron/characters.h>
#include <cauldron/strings.h>
#include <cauldron/vectors.h>

#include <tests/factories.h>
#include <tests/predicates.h>
#include <tests/operators.h>


TEST_CASE("strings \"Vectors\" strategy", "[Vectors]") {
  cauldron::Requirement<std::vector<std::string>> is_lower_vector(
      [&](const std::vector<std::string> &vector) -> bool {
        return std::all_of(vector.begin(),
                           vector.end(),
                           is_lower_string);
      });
  cauldron::Requirement<std::vector<std::string>> is_upper_vector(
      [&](const std::vector<std::string> &vector) -> bool {
        return std::all_of(vector.begin(),
                           vector.end(),
                           is_upper_string);
      });

  size_t max_length = constants::max_capacity;

  std::string non_zero_characters_string = factories::non_zero_characters();

  SECTION("single character alphabet") {
    cauldron::Just<size_t> sizes(1);
    cauldron::Just<size_t> lengths(1);
    for (char single_character: non_zero_characters_string) {
      std::string single_character_string{single_character};
      cauldron::Characters same_character(single_character_string);
      cauldron::Strings same_character_strings(lengths,
                                               same_character);
      cauldron::Vectors<std::string> same_character_strings_vectors(
          sizes,
          same_character_strings);

      auto vector = same_character_strings_vectors();

      REQUIRE(vector == std::vector<std::string>{single_character_string});
    }
  }

  SECTION("multiple characters alphabet") {
    size_t min_size = 0;
    size_t max_size = constants::max_capacity;
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    size_t min_length = 0;
    auto length_stays_in_range = in_range_checker<size_t>(min_length,
                                                          max_length);
    auto lengths_stay_in_range =
        [&](const std::vector<std::string> &vector) -> bool {
          return std::all_of(vector.begin(),
                             vector.end(),
                             [&](const std::string &string) -> bool {
                               return length_stays_in_range(string.length());
                             });
        };
    cauldron::Integers<size_t> sizes(min_size,
                                     max_size);
    cauldron::Integers<size_t> lengths(min_length,
                                       max_length);
    std::string alphabet_characters = factories::characters_string(
        constants::min_capacity,
        constants::max_capacity);
    cauldron::Characters alphabet(alphabet_characters);
    cauldron::Strings strings(lengths,
                              alphabet);
    cauldron::Vectors<std::string> strings_vectors(sizes,
                                                   strings);

    auto vector = strings_vectors();

    REQUIRE(size_stays_in_range(vector.size()));
    REQUIRE(lengths_stay_in_range(vector));
    REQUIRE(std::all_of(vector.begin(),
                        vector.end(),
                        [&](const std::string &string) -> bool {
                          return is_string_from_alphabet(string,
                                                         alphabet_characters);
                        }));
  }

  SECTION("filtration") {
    /* if ``min_length``/``min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty string/vector.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = min_size + 1;
    size_t min_length = constants::min_capacity;
    cauldron::Integers<size_t> sizes(min_size,
                                     max_size);
    cauldron::Integers<size_t> lengths(min_length,
                                       max_length);
    auto alphabetic_characters =
        cauldron::Characters(non_zero_characters_string).filter(is_alphabetic);
    cauldron::Strings alphabetic_strings(lengths,
                                         *alphabetic_characters);
    cauldron::Vectors<std::string> alphabetic(sizes,
                                              alphabetic_strings);

    SECTION("case") {
      auto lower_vectors = alphabetic.filter(is_lower_vector);
      auto upper_vectors = alphabetic.filter(is_upper_vector);

      auto lower_vector = (*lower_vectors)();
      auto upper_vector = (*upper_vectors)();
      auto sizes_stays_in_range = in_range_checker<size_t>(min_size,
                                                           max_size);
      auto length_stays_in_range = in_range_checker<size_t>(min_length,
                                                            max_length);
      auto lengths_stay_in_range =
          [&](const std::vector<std::string> &vector) -> bool {
            return std::all_of(vector.begin(),
                               vector.end(),
                               [&](const std::string &string) -> bool {
                                 return length_stays_in_range(string.length());
                               });
          };

      REQUIRE(sizes_stays_in_range(lower_vector.size()));
      REQUIRE(sizes_stays_in_range(upper_vector.size()));
      REQUIRE(lengths_stay_in_range(lower_vector));
      REQUIRE(lengths_stay_in_range(upper_vector));
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
    cauldron::Converter<std::vector<std::string>> to_lower_vector(
        [&](const std::vector<std::string> &vector) -> std::vector<std::string> {
          std::vector<std::string> result(vector.size());
          std::transform(vector.begin(),
                         vector.end(),
                         result.begin(),
                         to_lower_string);
          return result;
        });
    cauldron::Converter<std::vector<std::string>> to_upper_vector(
        [&](const std::vector<std::string> &vector) -> std::vector<std::string> {
          std::vector<std::string> result(vector.size());
          std::transform(vector.begin(),
                         vector.end(),
                         result.begin(),
                         to_upper_string);
          return result;
        });

    /* if ``min_length``/``min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty string/vector.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = min_size + 1;
    size_t min_length = constants::min_capacity;
    cauldron::Integers<size_t> sizes(min_size,
                                     max_size);
    cauldron::Integers<size_t> lengths(min_length,
                                       max_length);

    auto alphabetic_characters =
        cauldron::Characters(non_zero_characters_string).filter(is_alphabetic);
    cauldron::Strings alphabetic_strings(lengths,
                                         *alphabetic_characters);
    cauldron::Vectors<std::string> alphabetic(sizes,
                                              alphabetic_strings);

    SECTION("case") {
      auto lower_vectors = alphabetic.map(to_lower_vector);
      auto upper_vectors = alphabetic.map(to_upper_vector);

      auto lower_vector = lower_vectors();
      auto upper_vector = upper_vectors();
      auto sizes_stays_in_range = in_range_checker<size_t>(min_size,
                                                           max_size);
      auto length_stays_in_range = in_range_checker<size_t>(min_length,
                                                            max_length);
      auto lengths_stay_in_range =
          [&](const std::vector<std::string> &vector) -> bool {
            return std::all_of(vector.begin(),
                               vector.end(),
                               [&](const std::string &string) -> bool {
                                 return length_stays_in_range(string.length());
                               });
          };

      REQUIRE(sizes_stays_in_range(lower_vector.size()));
      REQUIRE(sizes_stays_in_range(upper_vector.size()));
      REQUIRE(lengths_stay_in_range(lower_vector));
      REQUIRE(lengths_stay_in_range(upper_vector));
      REQUIRE(is_lower_vector(lower_vector));
      REQUIRE(is_upper_vector(upper_vector));
    }

    SECTION("impossible") {
      auto invalid_lower_vectors =
          alphabetic.map(to_upper_vector).filter(is_lower_vector);
      auto invalid_upper_vectors =
          alphabetic.map(to_lower_vector).filter(is_upper_vector);

      REQUIRE_THROWS_AS((*invalid_lower_vectors)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_upper_vectors)(),
                        cauldron::OutOfCycles);
    }
  }
}
