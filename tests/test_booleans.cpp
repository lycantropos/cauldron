#include <catch.hpp>
#include <iostream>
#include "../cauldron/booleans.h"
#include "utils.h"


TEST_CASE("\"booleans\" strategy", "[booleans]") {
  strategies::Booleans false_values(0.);
  strategies::Booleans true_values(1.);

  SECTION("probability") {
    REQUIRE(not false_values());
    REQUIRE(true_values());
  }

  SECTION("filtration") {
    SECTION("identical") {
      auto still_false_values = false_values.filter(negate);
      auto still_true_values = true_values.filter(identity);

      REQUIRE((*still_false_values)() == false_values());
      REQUIRE((*still_true_values)() == true_values());
    }

    SECTION("impossible") {
      auto invalid_false_values = false_values.filter(identity);
      auto invalid_true_values = true_values.filter(negate);

      REQUIRE_THROWS_AS((*invalid_false_values)(),
                        strategies::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_true_values)(),
                        strategies::OutOfCycles);
    }
  }
}
