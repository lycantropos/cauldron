#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include <functional>
#include "bases.h"


namespace strategies {
/**
 * Strategy which generates pseudo-random integer values.
 * @tparam Value: type of values generated by strategy.
 */
template<typename Value>
class Integers : public CloneHelper<Value, Integers<Value>> {
  static_assert(std::is_integral<Value>::value,
                "``Value`` should have integral type");
 public:
  /**
   * @param min_value: minimum possible integer value.
   * @param max_value: maximum possible integer value.
   */
  explicit Integers(Value min_value = std::numeric_limits<Value>::min(),
                    Value max_value = std::numeric_limits<Value>::max())
      : min_value_(min_value),
        max_value_(max_value) {};

  /**
   * Generates pseudo-random integer value.
   */
  Value operator()() const override {
    static std::random_device random_device;
    auto distribution = std::uniform_int_distribution<Value>(min_value_,
                                                             max_value_);
    auto result = distribution(random_device);
    return result;
  }

 private:
  Value min_value_;
  Value max_value_;
};
}
