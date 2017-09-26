#include <stdexcept>
#include <cassert>
#include "characters.h"


namespace strategies {
void validate_characters(const std::string &characters) {
  if (characters.length() == 0) {
    throw std::invalid_argument("Characters container should not be empty.");
  };
}


Characters::Characters(const std::string &whitelist_characters,
                       unsigned max_attempts) {
  validate_characters(whitelist_characters);
  characters_ = whitelist_characters;
  max_attempts_ = max_attempts;
}


Characters::Characters(const std::string &whitelist_characters,
                       const std::vector<utils::Predicate<char>> &predicates,
                       unsigned int max_attempts) {
  validate_characters(whitelist_characters);
  characters_ = whitelist_characters;
  predicates_ = predicates;
  max_attempts_ = max_attempts;
}


Characters::Characters(const char whitelist_characters[]) {
  const auto &characters = std::string(whitelist_characters);
  validate_characters(characters);
  characters_ = characters;
}


Characters Characters::filter(const utils::Predicate<char> &predicate) const {
  auto predicates = std::vector<utils::Predicate<char>>(predicates_);
  predicates.push_back(predicate);
  return Characters(characters_,
                    predicates,
                    max_attempts_);
}


bool Characters::satisfactory(char character) const {
  return utils::primitive_satisfies_predicates<char>(character,
                                                     predicates_);
}


char Characters::operator()() const {
  if (characters_.length() == 1) {
    return characters_[0];
  }
  /* More at
   * http://www.cplusplus.com/reference/string/string/max_size/
   */
  auto non_negative_integers = Integers<size_t>(0);
  size_t max_index = characters_.length() - 1;
  for (unsigned _ = 0; _ < max_attempts_; ++_) {
    auto non_negative_integer = non_negative_integers();
    auto index = utils::positive_modulo<size_t>(non_negative_integer,
                                                max_index);
    assert(0 <= index <= max_index);
    char result = characters_[index];
    if (not satisfactory(result)) {
      continue;
    }
    return result;
  }
  throw OutOfTries(max_attempts_);
}
}
