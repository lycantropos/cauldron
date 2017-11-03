#include <catch.hpp>
#include <cauldron/booleans.h>
#include "predicates.h"


TEST_CASE("\"Booleans\" strategy", "[Booleans]") {
  cauldron::Booleans false_values(0.);
  cauldron::Booleans true_values(1.);

  SECTION("probability") {
    REQUIRE(!false_values());
    REQUIRE(true_values());
  }

  SECTION("filtration") {
    SECTION("identical") {
      auto still_false_values = false_values.filter(negate);
      auto still_true_values = true_values.filter(identity);

      REQUIRE(!(*still_false_values)());
      REQUIRE((*still_true_values)());
    }

    SECTION("impossible") {
      auto invalid_false_values = false_values.filter(identity);
      auto invalid_true_values = true_values.filter(negate);

      REQUIRE_THROWS_AS((*invalid_false_values)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_true_values)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    auto still_false_values = false_values.map(identity);
    auto still_true_values = true_values.map(identity);
    auto now_true_values = still_false_values->map(negate);
    auto now_false_values = still_true_values->map(negate);

    REQUIRE(!(*still_false_values)());
    REQUIRE((*still_true_values)());
    REQUIRE((*now_true_values)());
    REQUIRE(!(*now_false_values)());
  }
}
