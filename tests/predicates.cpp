#include <algorithm>
#include "predicates.h"


bool identity(bool boolean) {
  return boolean;
}


bool negate(bool boolean) {
  return not boolean;
}


bool is_alphabetic(char character) {
  return std::isalpha(character) != 0;
}


bool is_lower(char character) {
  return std::islower(character) != 0;
}


bool is_upper(char character) {
  return std::isupper(character) != 0;
}


bool is_lower_string(const std::string &string) {
  return std::all_of(string.begin(),
                     string.end(),
                     is_lower);
}


bool is_upper_string(const std::string &string) {
  return std::all_of(string.begin(),
                     string.end(),
                     is_upper);
}


bool is_character_in_string(char character,
                            const std::string &string) {
  return string.find(character) != std::string::npos;
}


bool is_string_from_alphabet(const std::string &string,
                             const std::string &alphabet_characters) {
  auto is_character_from_alphabet = [=](char character) -> bool {
    return is_character_in_string(character, alphabet_characters);
  };
  return std::all_of(string.begin(),
                     string.end(),
                     is_character_from_alphabet);
}
