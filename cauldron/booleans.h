#pragma once

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
  explicit Booleans(double probability = 0.5);

  /**
   * Generates pseudo-random ``bool`` value.
   */
  bool operator()() const override;

 private:
  double probability_;
};
}
