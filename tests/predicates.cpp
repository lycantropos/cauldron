#include "predicates.h"


bool is_character_in_string(char character,
                            const std::string &string) {
  return string.find(character) != -1;
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


bool identity(bool boolean) {
  return boolean;
}


bool negate(bool boolean) {
  return not boolean;
}
