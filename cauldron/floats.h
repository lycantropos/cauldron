#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include <functional>
#include "bases.h"


namespace cauldron {
template<typename Value>
void validate_floats(Value min_value,
                     Value max_value) {
  if (max_value - min_value > std::numeric_limits<Value>::max()) {
    throw std::invalid_argument(
        "Difference between ``max_value`` and ``min_value`` "
            "should not be greater than max possible value for type."
    );
  }
}


/**
 * Strategy which generates pseudo-random floating point values.
 * @tparam Value: type of values generated by strategy.
 */
template<typename Value>
class Floats : public CloneHelper<Value, Floats<Value>> {
  static_assert(std::is_floating_point<Value>(),
                "``Value`` should have floating point type.");
 public:
  /**
   * @param min_value: minimum possible floating point value.
   * @param max_value: maximum possible floating point value.
   */
  explicit Floats(Value min_value = 0.,
                  Value max_value = 1.) {
    validate_floats<Value>(min_value,
                           max_value);
    min_value_ = min_value;
    max_value_ = max_value;
  };

  /**
   * Generates pseudo-random floating point value.
   */
  Value operator()() const override {
    static std::random_device random_device;
    auto distribution = std::uniform_real_distribution<Value>(min_value_,
                                                              max_value_);
    auto result = distribution(random_device);
    return result;
  }

 private:
  Value min_value_;
  Value max_value_;
};
}
