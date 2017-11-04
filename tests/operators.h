#pragma once

#include <functional>


template<typename Number>
Number step_to_one(Number number) {
  if (number > 0) {
    number--;
  } else {
    number++;
  }
  return number;
}


template<typename Number>
std::function<Number(Number)> to_even_operator(Number min_number,
                                               Number max_number) {
  static_assert(std::is_integral<Number>(),
                "``Number`` should be integral type.");

  if (min_number >= max_number) {
    throw std::invalid_argument("``min_number`` should be less "
                                    "than ``max_number``.");
  }

  return [=](Number number) -> Number {
    number = step_to_one(number);
    number &= ~1;
    if (number > max_number) {
      return number - 2;
    } else if (number < min_number) {
      return number + 2;
    }
    return number;
  };
}


template<typename Number>
std::function<Number(Number)> to_odd_operator(Number min_number,
                                              Number max_number) {
  static_assert(std::is_integral<Number>(),
                "``Number`` should be integral type.");

  if (min_number >= max_number) {
    throw std::invalid_argument("``min_number`` should be less "
                                    "than ``max_number``.");
  }

  return [=](Number number) -> Number {
    number &= ~1;
    number = step_to_one(number);
    if (number > max_number) {
      return number - 2;
    } else if (number < min_number) {
      return number + 2;
    }
    return number;
  };
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
