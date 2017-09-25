#include <stdexcept>
#include <cassert>
#include "characters.h"
#include "utils.h"


namespace strategies {
void validate_characters(const std::string &characters) {
  if (characters.length() == 0) {
    throw std::invalid_argument("Characters container should not be empty.");
  };
}


Characters::Characters(const std::string &whitelist_characters) {
  validate_characters(whitelist_characters);
  characters_ = whitelist_characters;
}


Characters::Characters(const char whitelist_characters[]) {
  const auto &characters = std::string(whitelist_characters);
  validate_characters(characters);
  characters_ = characters;
}


char Characters::operator()() const {
  if (characters_.length() == 1) {
    return characters_[0];
  }
  /* More at
   * http://www.cplusplus.com/reference/string/string/max_size/
   */
  auto non_negative_integers = Integers<size_t>(0);
  auto non_negative_integer = non_negative_integers();
  size_t max_index = characters_.length() - 1;
  auto index = utils::positive_modulo<size_t>(non_negative_integer,
                                              max_index);
  assert(0 <= index <= max_index);
  return characters_[index];
}
}
