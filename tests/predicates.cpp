#include <iostream>
#include <catch.hpp>
#include "predicates.h"


bool identity(bool boolean) {
  return boolean;
}


bool negate(bool boolean) {
  return not boolean;
}


bool is_lower(char character) {
  return std::islower(character) != 0;
}


bool is_upper(char character) {
  return std::isupper(character) != 0;
}


bool is_digit(char character) {
  return std::isdigit(character) != 0;
}


bool is_alphabetic(char character) {
  return std::isalpha(character) != 0;
}


bool is_alphanumeric(char character) {
  return std::isalnum(character) != 0;
}


bool is_character_in_string(char character,
                            const std::string &string) {
  return string.find(character) != -1;
}


bool is_string_from_alphabet(const std::string &string,
                             const std::string &alphabet_characters) {
  auto is_character_from_alphabet = [=](char character) -> bool {
    return is_character_in_string(character, alphabet_characters);
  };
  return all_of(string.begin(),
                string.end(),
                is_character_from_alphabet);
}
