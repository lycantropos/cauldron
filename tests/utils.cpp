#include <cstddef>
#include <cmath>
#include "utils.h"


size_t sufficient_capacity(size_t favorable_outcomes_count,
                           size_t total_outcomes_count,
                           size_t attempts_count) {
  /**
   * Returns maximum possible capacity
   * for which all container's elements will be favorable
   * assuming that attempts are dependent by formula
   *    attempts_count * (probability ^ sufficient_capacity) >= 1
   * where
   *    probability = favorable_outcomes_count / total_outcomes_count
   * */
  // base equals to inverse probability
  double base = double(total_outcomes_count) / favorable_outcomes_count;
  return size_t((log(attempts_count) / log(base)));
}
