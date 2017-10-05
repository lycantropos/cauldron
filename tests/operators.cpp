#include <string>
#include <algorithm>
#include "operators.h"


char to_upper(char character) {
  return char(std::toupper(character));
}


char to_lower(char character) {
  return char(std::tolower(character));
}


std::string to_upper_string(const std::string &string) {
  std::string result;
  std::transform(string.begin(),
                 string.end(),
                 std::back_inserter(result),
                 to_upper);
  return result;
}


std::string to_lower_string(const std::string &string) {
  std::string result;
  std::transform(string.begin(),
                 string.end(),
                 std::back_inserter(result),
                 to_lower);
  return result;
}
