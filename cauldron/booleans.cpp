#include "booleans.h"


namespace strategies {
Booleans::Booleans(double probability)
    : probability_(probability) {}


bool Booleans::operator()() const {
  static std::random_device random_device;
  auto distribution = std::bernoulli_distribution(probability_);
  return distribution(random_device);
}
}
