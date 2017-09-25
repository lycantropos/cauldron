#include <catch.hpp>
#include <iostream>
#include "../cauldron/booleans.h"


TEST_CASE("\"booleans\" strategy", "[booleans]") {
  SECTION("probability") {
    strategies::Booleans false_values(0.);
    strategies::Booleans true_values(1.);

    REQUIRE(not false_values());
    REQUIRE(true_values());
  }
}
