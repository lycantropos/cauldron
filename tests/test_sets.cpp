#include <catch.hpp>
#include <iostream>
#include "../cauldron/just.h"
#include "../cauldron/booleans.h"
#include "../cauldron/integers.h"
#include "../cauldron/characters.h"
#include "../cauldron/strings.h"
#include "../cauldron/sets.h"
#include "factories.h"
#include "predicates.h"
#include "utils.h"
#include "operators.h"


static void check_booleans_sets_strategy() {
  auto is_false_set = [&](std::set<bool> set) -> bool {
    return std::all_of(set.begin(),
                       set.end(),
                       negate);
  };
  auto is_true_set = [&](std::set<bool> set) -> bool {
    return std::all_of(set.begin(),
                       set.end(),
                       identity);
  };

  SECTION("single element domain") {
    size_t min_size = 0;
    size_t max_size = 1;
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto true_values = std::make_shared<strategies::Booleans>(1.);
    auto false_values = std::make_shared<strategies::Booleans>(0.);
    strategies::Sets<bool> true_sets(sizes,
                                     true_values);
    strategies::Sets<bool> false_sets(sizes,
                                      false_values);

    auto true_set = true_sets();
    auto false_set = false_sets();
    auto stays_in_range = in_range_checker<size_t>(min_size,
                                                   max_size);

    REQUIRE(stays_in_range(true_set.size()));
    REQUIRE(stays_in_range(false_set.size()));
    REQUIRE(std::all_of(true_set.begin(),
                        true_set.end(),
                        identity));
    REQUIRE(std::all_of(false_set.begin(),
                        false_set.end(),
                        negate));
  }

  SECTION("filtration") {
    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = 2; // true or false;
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto booleans = std::make_shared<strategies::Booleans>();
    strategies::Sets<bool> booleans_sets(sizes,
                                         booleans);

    SECTION("truthfulness") {
      auto true_sets = booleans_sets.filter(is_true_set);
      auto false_sets = booleans_sets.filter(is_false_set);

      auto true_set = (*true_sets)();
      auto false_set = (*false_sets)();
      auto stays_in_range = in_range_checker(min_size,
                                             max_size);

      REQUIRE(stays_in_range(true_set.size()));
      REQUIRE(stays_in_range(false_set.size()));
      REQUIRE(is_true_set(true_set));
      REQUIRE(is_false_set(false_set));
    }

    SECTION("impossible") {
      auto invalid_sets =
          booleans_sets.filter(is_true_set)->filter(is_false_set);
      REQUIRE_THROWS_AS((*invalid_sets)(),
                        strategies::OutOfCycles);
    }
  }

  SECTION("mapping") {
    strategies::Converter<std::set<bool>> to_false_set(
        [&](const std::set<bool> &set) -> std::set<bool> {
          std::vector<bool> vector(set.size());
          std::fill(vector.begin(),
                    vector.end(),
                    false);
          return std::set<bool>(vector.begin(),
                                vector.end());
        });
    strategies::Converter<std::set<bool>> to_true_set(
        [&](const std::set<bool> &set) -> std::set<bool> {
          std::vector<bool> vector(set.size());
          std::fill(vector.begin(),
                    vector.end(),
                    true);
          return std::set<bool>(vector.begin(),
                                vector.end());
        });

    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    size_t min_size = constants::min_capacity;
    size_t max_size = 2; // true or false;
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    auto booleans = std::make_shared<strategies::Booleans>();
    strategies::Sets<bool> booleans_sets(sizes,
                                         booleans);

    SECTION("truthfulness") {
      auto false_sets = booleans_sets.map(to_false_set);
      auto true_sets = booleans_sets.map(to_true_set);

      auto false_set = (*false_sets)();
      auto true_set = (*true_sets)();
      auto stays_in_range = in_range_checker(min_size,
                                             max_size);

      REQUIRE(stays_in_range(false_set.size()));
      REQUIRE(stays_in_range(true_set.size()));
      REQUIRE(is_false_set(false_set));
      REQUIRE(is_true_set(true_set));
    }

    SECTION("impossible") {
      auto invalid_false_sets =
          booleans_sets.map(to_false_set)->filter(is_true_set);
      auto invalid_true_sets =
          booleans_sets.map(to_false_set)->filter(is_true_set);
      REQUIRE_THROWS_AS((*invalid_false_sets)(),
                        strategies::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_true_sets)(),
                        strategies::OutOfCycles);
    }
  }
}


template<typename T>
static void check_integers_sets_strategy() {
  auto is_even_set = [&](std::set<T> set) -> bool {
    return std::all_of(set.begin(),
                       set.end(),
                       even<T>);
  };
  auto is_odd_set = [&](std::set<T> set) -> bool {
    return std::all_of(set.begin(),
                       set.end(),
                       odd<T>);
  };

  T min_value = std::numeric_limits<T>::min();
  T max_value = std::numeric_limits<T>::max();
  auto integers = std::make_shared<strategies::Integers<T>>(min_value,
                                                            max_value);
  auto integer_stays_in_range = in_range_checker<T>(min_value,
                                                    max_value);
  auto integers_stay_in_range =
      [&](const std::set<T> &integers_set) -> bool {
        return std::all_of(integers_set.begin(),
                           integers_set.end(),
                           integer_stays_in_range);
      };

  SECTION("single element domain") {
    // ``signed char`` is the smallest tested integer type
    static int min_integer = std::numeric_limits<signed char>::min();
    static int max_integer = std::numeric_limits<signed char>::max();
    static const auto integers_range = factories::integers_range<int>(
        min_integer,
        max_integer);
    strategies::Just<size_t> ones(1);
    for (T integer: integers_range) {
      auto single_integer_set = std::set<T>{integer};
      strategies::Just<T> same_integer(integer);
      strategies::Sets<T> same_integer_sets(
          std::make_shared<strategies::Just<size_t>>(ones),
          std::make_shared<strategies::Just<T>>(same_integer));

      auto set = same_integer_sets();

      bool sets_are_equal = set == single_integer_set;
      REQUIRE(sets_are_equal);
    }
  }

  SECTION("multiple elements domain") {
    static size_t min_size = 0;
    static size_t max_size = constants::max_capacity;
    static const std::shared_ptr<strategies::Integers<size_t>> &sizes =
        std::make_shared<strategies::Integers<size_t>>(min_size,
                                                       max_size);
    strategies::Sets<T> integers_sets(sizes,
                                      integers);
    auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                        max_size);
    auto set = integers_sets();

    REQUIRE(size_stays_in_range(set.size()));
    REQUIRE(integers_stay_in_range(set));
  }

  SECTION("filtration") {
    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    static size_t min_size = constants::min_capacity;
    static size_t max_size = sufficient_capacity(1, 2, // odd or even
                                                 strategies::MAX_CYCLES);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    strategies::Sets<T> sets(sizes,
                             integers);

    SECTION("parity") {
      auto even_sets = sets.filter(is_even_set);
      auto odd_sets = sets.filter(is_odd_set);

      auto even_set = (*even_sets)();
      auto odd_set = (*odd_sets)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(even_set.size()));
      REQUIRE(size_stays_in_range(odd_set.size()));
      REQUIRE(integers_stay_in_range(even_set));
      REQUIRE(integers_stay_in_range(odd_set));
      REQUIRE(is_even_set(even_set));
      REQUIRE(is_odd_set(odd_set));
    }

    SECTION("impossible") {
      auto invalid_sets =
          sets.filter(is_even_set)->filter(is_odd_set);

      REQUIRE_THROWS_AS((*invalid_sets)(),
                        strategies::OutOfCycles);
    }
  }

  SECTION("mapping") {
    strategies::Converter<std::set<T>> to_even_set(
        [&](const std::set<T> &set) -> std::set<T> {
          std::vector<T> vector;
          vector.reserve(set.size());
          std::transform(set.begin(),
                         set.end(),
                         std::back_inserter(vector),
                         to_even<T>);
          return std::set<T>(vector.begin(),
                             vector.end());
        });
    strategies::Converter<std::set<T>> to_odd_set(
        [&](const std::set<T> &set) -> std::set<T> {
          std::vector<T> vector;
          vector.reserve(set.size());
          std::transform(set.begin(),
                         set.end(),
                         std::back_inserter(vector),
                         to_odd<T>);
          return std::set<T>(vector.begin(),
                             vector.end());
        });

    /* if `min_size`` equals to zero
     * than "impossible" section would not raise exception
     * since it is possible to avoid filters with empty set.
     */
    static size_t min_size = constants::min_capacity;
    static size_t max_size = sufficient_capacity(1, 2, // odd or even
                                                 strategies::MAX_CYCLES);
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    strategies::Sets<T> sets(sizes,
                             integers);

    SECTION("parity") {
      auto even_sets = sets.map(to_even_set);
      auto odd_sets = sets.map(to_odd_set);

      auto even_set = (*even_sets)();
      auto odd_set = (*odd_sets)();
      auto size_stays_in_range = in_range_checker<size_t>(min_size,
                                                          max_size);

      REQUIRE(size_stays_in_range(even_set.size()));
      REQUIRE(size_stays_in_range(odd_set.size()));
      REQUIRE(integers_stay_in_range(even_set));
      REQUIRE(integers_stay_in_range(odd_set));
      REQUIRE(is_even_set(even_set));
      REQUIRE(is_odd_set(odd_set));
    }

    SECTION("impossible") {
      auto invalid_even_sets =
          sets.map(to_odd_set)->filter(is_even_set);
      auto invalid_odd_sets =
          sets.map(to_even_set)->filter(is_odd_set);

      REQUIRE_THROWS_AS((*invalid_even_sets)(),
                        strategies::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_odd_sets)(),
                        strategies::OutOfCycles);
    }
  }
}


static void check_characters_sets_strategy() {
  auto is_lower_set = [&](const std::set<char> set) -> bool {
    return std::all_of(set.begin(),
                       set.end(),
                       is_lower);
  };
  auto is_upper_set = [&](const std::set<char> set) -> bool {
    return std::all_of(set.begin(),
                       set.end(),
                       is_upper);
  };

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
    size_t min_size = 0;
    size_t max_size = constants::max_capacity / 4;
    auto sizes = std::make_shared<strategies::Integers<size_t>>(min_size,
                                                                max_size);
    std::string characters_string = factories::characters_string();
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
    size_t max_size = constants::alphabetic_characters_count / 4;
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
    size_t max_size = constants::max_capacity / 4;
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


static void check_strings_sets_strategy() {
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
          std::all_of(set.begin(),
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
            std::all_of(set.begin(),
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
            std::all_of(set.begin(),
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


TEST_CASE("\"sets\" strategy", "[sets]") {
  SECTION("bool") {
    check_booleans_sets_strategy();
  }

  SECTION("unsigned char") {
    check_integers_sets_strategy<unsigned char>();
  }

  SECTION("signed char") {
    check_integers_sets_strategy<signed char>();
  }

  SECTION("short int") {
    check_integers_sets_strategy<short>();
  }

  SECTION("unsigned short int") {
    check_integers_sets_strategy<unsigned short>();
  }

  SECTION("int") {
    check_integers_sets_strategy<int>();
  }

  SECTION("unsigned int") {
    check_integers_sets_strategy<unsigned>();
  }

  SECTION("long int") {
    check_integers_sets_strategy<long>();
  }

  SECTION("unsigned long int") {
    check_integers_sets_strategy<unsigned long>();
  }

  SECTION("long long int") {
    check_integers_sets_strategy<long long>();
  }

  SECTION("unsigned long long int") {
    check_integers_sets_strategy<unsigned long long>();
  }

  SECTION("characters") {
    check_characters_sets_strategy();
  }

  SECTION("strings") {
    check_strings_sets_strategy();
  }
}
