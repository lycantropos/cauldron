#include <cmath>
#include <limits>
#include <stdexcept>

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


size_t sufficient_set_size(size_t total_count, size_t attempts_count) {
  size_t result = 1;
  size_t total_count_exponent = total_count;
  while (result < total_count &&
      combinations(total_count,
                   result) * attempts_count >= total_count_exponent) {
    result++;
    total_count_exponent *= total_count;
  }
  return result;
}


/*
 * from https://stackoverflow.com/a/4701106/5997596
 */
size_t combinations(size_t total_count, size_t target_count) {
  if (target_count > total_count) {
    throw std::invalid_argument(
        "Target elements count should not be greater than total count."
    );
  }
  static size_t max_value = std::numeric_limits<unsigned long>::max();
  size_t result = 1;
  for (size_t denominator = 1;
       denominator <= target_count;
       ++denominator, --total_count) {
    size_t common_divisor = gcd(result, denominator);
    result /= common_divisor;
    size_t step = total_count / (denominator / common_divisor);
    if (result > max_value / step)
      throw std::overflow_error("Overflow in combinations.");
    result *= step;
  }
  return result;
}


size_t gcd(size_t x, size_t y) {
  while (y != 0) {
    size_t remainder = x % y;
    x = y;
    y = remainder;
  }
  return x;
}
