#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include <functional>
#include "bases.h"


namespace strategies {
/**
 * Strategy which generates pseudo-random ``T`` integer values.
 * @tparam T: type of data generated by strategy.
 */
template<typename T>
class Integers : public CloneHelper<T, Integers<T>> {
 public:
  /**
   * @param min_value: minimum possible integer value.
   * @param max_value: maximum possible integer value.
   */
  explicit Integers(T min_value = std::numeric_limits<T>::min(),
                    T max_value = std::numeric_limits<T>::max())
      : min_value_(min_value),
        max_value_(max_value) {};

  /**
   * Generates pseudo-random ``T`` integer values.
   */
  T operator()() const override {
    static std::random_device random_device;
    auto distribution = std::uniform_int_distribution<T>(min_value_,
                                                         max_value_);
    auto result = distribution(random_device);
    return result;
  }

 private:
  T min_value_;
  T max_value_;
};
}
