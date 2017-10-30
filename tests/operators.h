#pragma once

#include <functional>


template<typename Number>
Number to_even(Number number) {
  static_assert(std::is_integral<Number>(),
                "``Number`` should be integral type.");
  return Number(number / 2) * 2;
}


template<typename Number>
Number to_odd(Number number) {
  static_assert(std::is_integral<Number>(),
                "``Number`` should be integral type.");
  Number sign = number < 0 ? -1 : 1;
  return Number(number / 2 - sign) * 2 + sign;
}


template<typename Number>
std::function<Number(Number)> to_positive_operator(Number max_number) {
  static_assert(std::is_signed<Number>(),
                "``Number`` should be signed arithmetic type.");

  if (max_number <= 0) {
    throw std::invalid_argument(
        "``max_number`` should be positive number."
    );
  }

  return [=](Number number) -> Number {
    if (number == 0) {
      return max_number;
    }
    return std::min(std::abs(number), max_number);
  };
}


template<typename Number>
std::function<Number(Number)> to_non_positive_operator(Number min_number) {
  static_assert(std::is_signed<Number>(),
                "``Number`` should be signed arithmetic type.");

  if (min_number > 0) {
    throw std::invalid_argument(
        "``min_number`` should be non-positive number."
    );
  }

  return [=](Number number) -> Number {
    return std::max(-std::abs(number), min_number);
  };
}


char to_upper(char character);

char to_lower(char character);

std::string to_upper_string(const std::string &string);

std::string to_lower_string(const std::string &string);
