#include "factories.h"


std::string factories::characters_string(size_t min_length,
                                         size_t max_length) {
  static std::random_device random_device;

  auto lengths = std::uniform_int_distribution<size_t>(min_length,
                                                       max_length);
  auto characters = std::uniform_int_distribution<char>();

  auto length = lengths(random_device);
  std::string characters_string;
  for (size_t _ = 0; _ < length; ++_) {
    characters_string.push_back(characters(random_device));
  }
  return characters_string;
}


std::string factories::non_zero_characters(char min_character,
                                           char max_character) {
  std::string non_zero_characters;
  for (char character = min_character;
       character < max_character;
       character = ++character + (character == 0)) {
    non_zero_characters.push_back(character);
  }
  return non_zero_characters;
}
