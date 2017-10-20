#pragma once

#include <vector>
#include <functional>
#include <set>


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

bool is_string_from_alphabet(const std::string &string,
                             const std::string &alphabet_characters);

bool is_lower(char character);

bool is_upper(char character);

bool is_alphabetic(char character);

bool is_lower_string(const std::string &string);

bool is_upper_string(const std::string &string);


template<typename T>
bool is_object_in_vector(T object,
                         const std::vector<T> &vector) {
  return std::find(vector.begin(), vector.end(), object) != vector.end();
};


template<typename T>
bool is_object_in_set(T object,
                      const std::set<T> &set) {
  return set.find(object) != set.end();
};
