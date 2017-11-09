#include <catch.hpp>
#include <cauldron/booleans.h>

#include <tests/statistics.h>


static auto booleans_factory() {
  static std::random_device random_device;
  std::uniform_real_distribution<double> distribution;
  auto probability = distribution(random_device);
  return cauldron::Booleans(probability);
}


TEST_CASE("booleans \"Union\" strategy", "[Union]") {
  auto booleans = booleans_factory();
  auto other_booleans = booleans_factory();
  auto another_booleans = booleans_factory();

  SECTION("idempotence") {
    auto union_with_self = booleans || booleans;

    bool strategies_are_similar =
        are_generators_similar<bool, double>(booleans,
                                             union_with_self,
                                             false,
                                             true);
    REQUIRE(strategies_are_similar);
  }

  SECTION("commutativity") {
    auto booleans_union = booleans || other_booleans;
    auto reverse_booleans_union = other_booleans || booleans;

    bool strategies_are_similar =
        are_generators_similar<bool, double>(booleans_union,
                                             reverse_booleans_union,
                                             false,
                                             true);
    REQUIRE(strategies_are_similar);
  }

  SECTION("associativity") {
    auto left_booleans_union =
        (booleans || other_booleans) || another_booleans;
    auto right_booleans_union =
        booleans || (other_booleans || another_booleans);

    bool strategies_are_similar =
        are_generators_similar<bool, double>(left_booleans_union,
                                             right_booleans_union,
                                             false,
                                             true);
    REQUIRE(strategies_are_similar);
  }
}
