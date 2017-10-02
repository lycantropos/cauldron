#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include <functional>
#include "bases.h"


namespace strategies {
class Booleans : public Filtered<bool> {
 public:
  explicit Booleans(double probability = 0.5,
                    const Sieve<bool> &sieve = Sieve<bool>())
      : probability_(probability),
        Filtered<bool>(sieve) {};

 private:
  double probability_;

  bool producer() const {
    static std::random_device random_device;
    auto distribution = std::bernoulli_distribution(probability_);
    auto result = distribution(random_device);
    return result;
  }

  std::unique_ptr<Filtered<bool>> update_sieve(
      const Sieve<bool> &sieve
  ) const override {
    return std::make_unique<Booleans>(probability_,
                                      sieve);
  }
};
}
