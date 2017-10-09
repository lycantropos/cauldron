#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include "bases.h"
#include "integers.h"


namespace strategies {
/**
 * Strategy which generates pseudo-random ``char`` values.
 */
class Characters : public CloneHelper<char, Characters> {
 public:
  explicit Characters(const std::string &domain);

  explicit Characters(const char domain[]);

  /**
   * Generates pseudo-random ``char`` value.
   */
  char operator()() const override;

 private:
  std::string domain_;
};
}
