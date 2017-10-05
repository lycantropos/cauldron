#pragma once


template<typename T>
T to_even(T number) {
  return (number / 2) * 2;
};


template<typename T>
T to_odd(T number) {
  if (number == 0 or number == 1) {
    return 1;
  }
  T sign = number < 0 ? -1 : 1;
  return (number / 2) * 2 - sign;
};


char to_upper(char character);


char to_lower(char character);


std::string to_upper_string(const std::string &string);


std::string to_lower_string(const std::string &string);
