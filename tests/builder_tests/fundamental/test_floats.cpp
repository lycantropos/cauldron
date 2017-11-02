#include <catch.hpp>
#include "../../../cauldron/just.h"
#include "../../../cauldron/integers.h"
#include "../../../cauldron/floats.h"
#include "../../../cauldron/builder.h"
#include "../../predicates.h"
#include "../../operators.h"
#include "../../ordered_pair.h"
#include "../wrapper.h"


template<typename Number>
static void check_strategy() {
  using FloatWrapper = Wrapper<Number>;

  cauldron::Requirement<FloatWrapper> is_positive_wrapper(
      [&](FloatWrapper wrapper) -> bool {
        return positive<Number>(wrapper.field());
      });
  cauldron::Requirement<FloatWrapper> is_non_positive_wrapper(
      [&](FloatWrapper wrapper) -> bool {
        return non_positive<Number>(wrapper.field());
      });

  Number min_number;
  Number max_number;
  std::tie(min_number, max_number) = ordered_pair(
      std::numeric_limits<Number>::lowest(),
      std::numeric_limits<Number>::max()
  );
  auto number_stays_in_range = in_range_checker<Number>(min_number,
                                                        max_number);
  auto numbers = std::make_shared<cauldron::Floats<Number>>(min_number,
                                                            max_number);

  cauldron::Builder<FloatWrapper, Number> numbers_wrappers(numbers);

  SECTION("stays in range") {
    auto wrapper = numbers_wrappers();

    REQUIRE(number_stays_in_range(wrapper.field()));
  }

  SECTION("union") {
    auto still_numbers_wrappers = numbers_wrappers || numbers_wrappers;

    auto wrapper = still_numbers_wrappers();

    REQUIRE(number_stays_in_range(wrapper.field()));
  }

  SECTION("filtration") {
    SECTION("sign") {
      auto positive_wrappers = numbers_wrappers.filter(is_positive_wrapper);
      auto non_positive_wrappers =
          numbers_wrappers.filter(is_non_positive_wrapper);

      auto positive_wrapper = (*positive_wrappers)();
      auto non_positive_wrapper = (*non_positive_wrappers)();

      REQUIRE(number_stays_in_range(positive_wrapper.field()));
      REQUIRE(number_stays_in_range(non_positive_wrapper.field()));
      REQUIRE(is_positive_wrapper(positive_wrapper));
      REQUIRE(is_non_positive_wrapper(non_positive_wrapper));
    }

    SECTION("impossible") {
      auto invalid_wrappers =
          numbers_wrappers.filter(is_positive_wrapper)
              ->filter(is_non_positive_wrapper);

      REQUIRE_THROWS_AS((*invalid_wrappers)(),
                        cauldron::OutOfCycles);
    }
  }

  SECTION("mapping") {
    auto to_positive = to_positive_operator(max_number);
    auto to_non_positive = to_non_positive_operator(min_number);
    cauldron::Converter<FloatWrapper> to_positive_wrapper(
        [&](const FloatWrapper &wrapper) -> FloatWrapper {
          return FloatWrapper(to_positive(wrapper.field()));
        });
    cauldron::Converter<FloatWrapper> to_non_positive_wrapper(
        [&](const FloatWrapper &wrapper) -> FloatWrapper {
          return FloatWrapper(to_non_positive(wrapper.field()));
        });

    SECTION("sign") {
      auto positive_wrappers = numbers_wrappers.map(to_positive_wrapper);
      auto non_positive_wrappers =
          numbers_wrappers.map(to_non_positive_wrapper);

      auto positive_wrapper = (*positive_wrappers)();
      auto non_positive_wrapper = (*non_positive_wrappers)();

      REQUIRE(number_stays_in_range(positive_wrapper.field()));
      REQUIRE(number_stays_in_range(non_positive_wrapper.field()));
      REQUIRE(is_positive_wrapper(positive_wrapper));
      REQUIRE(is_non_positive_wrapper(non_positive_wrapper));
    }

    SECTION("impossible") {
      auto invalid_positive_wrappers =
          numbers_wrappers.map(to_non_positive_wrapper)
              ->filter(is_positive_wrapper);
      auto invalid_non_positive_wrappers =
          numbers_wrappers.map(to_positive_wrapper)
              ->filter(is_non_positive_wrapper);

      REQUIRE_THROWS_AS((*invalid_positive_wrappers)(),
                        cauldron::OutOfCycles);
      REQUIRE_THROWS_AS((*invalid_non_positive_wrappers)(),
                        cauldron::OutOfCycles);
    }
  }
}


TEST_CASE("floats \"Builder\" strategy", "[Builder]") {
  SECTION("float") {
    check_strategy<float>();
  }

  SECTION("double") {
    check_strategy<double>();
  }

  SECTION("long double") {
    check_strategy<long double>();
  }
}
