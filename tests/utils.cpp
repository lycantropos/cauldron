#include "utils.h"


bool is_character_in_string(const char &character,
                            const std::string &string) {
  return string.find(character) != -1;
}
