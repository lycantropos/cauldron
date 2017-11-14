#include <catch.hpp>
#include <cauldron/just.h>
#include <cauldron/integers.h>
#include <cauldron/characters.h>
#include <cauldron/strings.h>

#include <tests/factories.h>
#include <tests/ordered_pair.h>
#include <tests/statistics.h>


static auto lengths_factory(size_t min_length,
                            size_t max_length) {
  std::tie(min_length, max_length) = ordered_pair(min_length, max_length);
  return cauldron::Integers<size_t>(min_length,
                                    max_length);
}


static auto alphabet_factory(size_t min_length,
                             size_t max_length) {
  std::tie(min_length, max_length) = ordered_pair(min_length, max_length);
  std::string alphabet_characters = factories::characters_string(min_length,
                                                                 max_length);
  return cauldron::Characters(alphabet_characters);
}


static auto lengths_generator(
    const cauldron::Strategy<std::string> &strings
) {
  return [&strings]() -> size_t {
    auto string = strings();
    return string.length();
  };
}


static auto characters_sampler(
    const cauldron::Strategy<std::string> &strings
) {
  return [&strings]() -> std::vector<char> {
    auto string = strings();
    return std::vector<char>(string.begin(),
                             string.end());
  };
}


TEST_CASE("strings \"Union\" strategy", "[Union]") {
  size_t min_length = 0;
  size_t max_length = constants::max_capacity;
  auto lengths = lengths_factory(min_length,
                                 max_length);
  auto other_lengths = lengths_factory(min_length,
                                       max_length);
  auto another_lengths = lengths_factory(min_length,
                                         max_length);
  auto alphabet = alphabet_factory(min_length,
                                   max_length);
  auto other_alphabet = alphabet_factory(min_length,
                                         max_length);
  auto another_alphabet = alphabet_factory(min_length,
                                           max_length);
  cauldron::Strings strings(lengths,
                            alphabet);
  cauldron::Strings other_strings(other_lengths,
                                  other_alphabet);
  cauldron::Strings another_strings(another_lengths,
                                    another_alphabet);

  SECTION("idempotence") {
    auto union_with_self = strings || strings;

    bool lengths_are_similar =
        are_generators_similar<size_t, double>(
            lengths_generator(strings),
            lengths_generator(union_with_self),
            false,
            true);
    bool characters_are_similar =
        are_samplers_similar<char, double>(
            characters_sampler(strings),
            characters_sampler(union_with_self),
            false,
            true);
    REQUIRE(lengths_are_similar);
    REQUIRE(characters_are_similar);
  }

  SECTION("commutativity") {
    auto strings_union = strings || other_strings;
    auto reverse_strings_union = other_strings || strings;

    bool lengths_are_similar =
        are_generators_similar<size_t, double>(
            lengths_generator(strings_union),
            lengths_generator(reverse_strings_union),
            false,
            true);
    bool characters_are_similar =
        are_samplers_similar<char, double>(
            characters_sampler(strings_union),
            characters_sampler(reverse_strings_union),
            false,
            true);
    REQUIRE(lengths_are_similar);
    REQUIRE(characters_are_similar);
  }

  SECTION("associativity") {
    auto left_strings_union =
        (strings || other_strings) || another_strings;
    auto right_strings_union =
        strings || (other_strings || another_strings);

    bool lengths_are_similar =
        are_generators_similar<size_t, double>(
            lengths_generator(left_strings_union),
            lengths_generator(right_strings_union),
            false,
            true);
    bool characters_are_similar =
        are_samplers_similar<char, double>(
            characters_sampler(left_strings_union),
            characters_sampler(right_strings_union),
            false,
            true);
    REQUIRE(lengths_are_similar);
    REQUIRE(characters_are_similar);
  }
}
