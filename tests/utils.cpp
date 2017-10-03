#include <cstddef>
#include "utils.h"


size_t sufficient_capacity(size_t favorable_outcomes_count,
                           size_t total_outcomes_count,
                           size_t attempts_count) {
  /**
   * Returns maximum possible capacity
   * for which all container's elements will be favorable
   * assuming that attempts are dependent.
   * */
  double probability = double(favorable_outcomes_count) / total_outcomes_count;
  size_t result = 0;
  double step = 1.;
  while ((step *= probability) * attempts_count >= 1.) {
    result++;
  }
  return result;
}
