#pragma once


template<typename Number>
Number to_even(Number number) {
  static_assert(std::is_integral<Number>::value,
                "``Number`` should have integral type.");
  return (number / 2) * 2;
}


template<typename Number>
Number to_odd(Number number) {
  static_assert(std::is_integral<Number>::value,
                "``Number`` should have integral type.");
  if (number == 0) {
    return 1;
  } else if (number == 1 or number == -1) {
    return number;
  }
  Number sign = number < 0 ? -1 : 1;
  return (number / 2) * 2 - sign;
}

char to_upper(char character);

char to_lower(char character);

std::string to_upper_string(const std::string &string);

std::string to_lower_string(const std::string &string);
