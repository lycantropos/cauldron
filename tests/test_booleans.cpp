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

  SECTION("filtration") {
    auto identity = [=](bool boolean) -> bool {
      return boolean;
    };
    auto negate = [=](bool boolean) -> bool {
      return not boolean;
    };

    SECTION("identical") {
      strategies::Booleans false_values(0.);
      strategies::Booleans true_values(1.);

      auto still_false_values = false_values.filter(negate);
      auto still_true_values = true_values.filter(identity);

      REQUIRE(still_false_values() == false_values());
      REQUIRE(still_true_values() == true_values());
    }

    SECTION("impossible") {
      strategies::Booleans false_values(0.);
      strategies::Booleans true_values(1.);

      auto invalid_false_values = false_values.filter(identity);
      auto invalid_true_values = true_values.filter(negate);

      REQUIRE_THROWS_AS(invalid_false_values(),
                        strategies::OutOfTries);
      REQUIRE_THROWS_AS(invalid_true_values(),
                        strategies::OutOfTries);
    }
  }
}
