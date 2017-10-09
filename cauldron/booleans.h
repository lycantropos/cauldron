#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include <functional>
#include "bases.h"


namespace strategies {
/**
 * Strategy which generates pseudo-random ``bool`` values
 * with defined probability.
 */
class Booleans : public CloneHelper<bool, Booleans> {
 public:
  /**
   * @param probability: how often ``true`` values will be generated
   * compared to ``false`` values.
   */
  explicit Booleans(double probability = 0.5)
      : probability_(probability) {};

  /**
   * Generates pseudo-random ``bool`` values.
   */
  bool operator()() const override {
    static std::random_device random_device;
    auto distribution = std::bernoulli_distribution(probability_);
    return distribution(random_device);
  }

 private:
  double probability_;
};
}
