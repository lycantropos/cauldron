#include <catch.hpp>
#include <cauldron/integers.h>
#include <tests/statistics.h>
#include <tests/ordered_pair.h>
#include <tests/factories.h>
#include <cauldron/characters.h>


static auto characters_factory(size_t min_length,
                               size_t max_length) {
  std::tie(min_length, max_length) = ordered_pair(min_length, max_length);
  std::string domain = factories::characters_string(min_length,
                                                    max_length);
  return cauldron::Characters(domain);
}


TEST_CASE("characters \"Union\" strategy", "[Union]") {
  size_t min_number;
  size_t max_number;
  std::tie(min_number, max_number) = ordered_pair<size_t>(
      constants::min_capacity,
      constants::max_capacity
  );

  auto characters = characters_factory(min_number,
                                       max_number);
  auto other_characters = characters_factory(min_number,
                                             max_number);
  auto another_characters = characters_factory(min_number,
                                               max_number);

  SECTION("idempotence") {
    auto union_with_self = characters || characters;

    bool strategies_are_similar =
        are_generators_similar<char, double>(characters,
                                             union_with_self,
                                             min_number,
                                             max_number);
    REQUIRE(strategies_are_similar);
  }

  SECTION("commutativity") {
    auto characters_union = characters || other_characters;
    auto reverse_characters_union = other_characters || characters;

    bool unions_are_similar =
        are_generators_similar<char, double>(characters_union,
                                             reverse_characters_union,
                                             min_number,
                                             max_number);
    REQUIRE(unions_are_similar);
  }

  SECTION("associativity") {
    auto left_characters_union =
        (characters || other_characters) || another_characters;
    auto right_characters_union =
        characters || (other_characters || another_characters);

    bool strategies_are_similar =
        are_generators_similar<char, double>(left_characters_union,
                                             right_characters_union,
                                             min_number,
                                             max_number);
    REQUIRE(strategies_are_similar);
  }
}
