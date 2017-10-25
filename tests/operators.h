#pragma once


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


char to_upper(char character);

char to_lower(char character);

std::string to_upper_string(const std::string &string);

std::string to_lower_string(const std::string &string);
