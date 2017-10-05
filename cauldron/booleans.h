#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include <functional>
#include "bases.h"


namespace strategies {
class Booleans : public CloneHelper<bool, Booleans> {
 public:
  explicit Booleans(double probability = 0.5)
      : probability_(probability) {};

  bool operator()() const override {
    static std::random_device random_device;
    auto distribution = std::bernoulli_distribution(probability_);
    return distribution(random_device);
  }

 private:
  double probability_;
};
}
