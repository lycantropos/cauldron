#pragma once

#include <iostream>
#include <vector>
#include <functional>


bool identity(bool boolean);;


bool negate(bool boolean);;


template<typename T>
std::function<bool(T)> in_range_checker(T min_value,
                                        T max_value) {
  return [=](T value) -> bool {
    return min_value <= value <= max_value;
  };
}


template<typename T>
bool even(T number) {
  return number % 2 == 0;
};


template<typename T>
bool odd(T number) {
  return number % 2 != 0;
};


bool is_character_in_string(char character,
                            const std::string &string);


bool is_lower(char character);


bool is_upper(char character);


bool is_digit(char character);


bool is_alphabetic(char character);


bool is_alphanumeric(char character);
