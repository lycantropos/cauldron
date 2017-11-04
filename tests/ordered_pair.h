#pragma once

#include <algorithm>
#include <random>
#include <tuple>


template<typename Number>
static auto ordered_pair_helper(std::true_type,
                                Number min_number,
                                Number max_number) {
  static_assert(std::is_integral<Number>(),
                "``Number`` should be arithmetic type.");

  if (min_number >= max_number) {
    throw std::invalid_argument("``min_number`` should be less "
                                    "than ``max_number``.");
  }

  static std::random_device random_device;
  Number quarter = max_number / 4 + min_number / 4;
  std::uniform_int_distribution<Number> left_number_distribution(
      min_number,
      std::max<Number>(min_number + 1, quarter)
  );
  Number left_number = left_number_distribution(random_device);
  std::uniform_int_distribution<Number> right_number_distribution(
      std::min<Number>(max_number - 1, 3 * quarter),
      max_number
  );
  Number right_number = right_number_distribution(random_device);
  return std::make_tuple(left_number, right_number);
}


template<typename Number>
Number abs(Number number) {
  if (number < 0) {
    return -number;
  }
  return number;
}


template<typename Number>
static auto ordered_pair_helper(std::false_type,
                                Number min_number,
                                Number max_number) {
  static_assert(std::is_floating_point<Number>(),
                "``Number`` should be floating point type.");

  if (min_number >= 0) {
    throw std::invalid_argument("``min_number`` should be negative.");
  }
  if (max_number <= 0) {
    throw std::invalid_argument("``max_number`` should be positive.");
  }

  Number max_modulus = std::min(abs(min_number + abs(min_number / 2)),
                                abs(max_number - abs(max_number / 2)));

  static std::random_device random_device;

  Number min_positive_number = std::numeric_limits<Number>::min();
  std::uniform_real_distribution<Number> right_number_distribution(
      std::max(min_positive_number, max_modulus / 2),
      max_modulus
  );
  Number right_number = right_number_distribution(random_device);
  Number left_number = -right_number;
  return std::make_tuple(left_number, right_number);
}


template<typename Number>
auto ordered_pair(Number min_number,
                  Number max_number) {
  static_assert(std::is_arithmetic<Number>(),
                "``Number`` should be arithmetic type.");

  return ordered_pair_helper(std::is_integral<Number>(),
                             min_number,
                             max_number);
}
