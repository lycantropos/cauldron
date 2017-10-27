#include <stdexcept>
#include <random>
#include "characters.h"


namespace strategies {
void validate_characters(const std::string &characters) {
  if (characters.length() == 0) {
    throw std::invalid_argument("Characters container should not be empty.");
  };
}


Characters::Characters(const std::string &domain) {
  validate_characters(domain);
  domain_ = domain;
}


Characters::Characters(const char domain[]) {
  const auto &domain_string = std::string(domain);
  validate_characters(domain_string);
  domain_ = domain_string;
}


char Characters::operator()() const {
  if (domain_.length() == 1) {
    return domain_[0];
  }
  static std::random_device random_device;
  size_t max_index = domain_.length() - 1;
  auto distribution = std::uniform_int_distribution<size_t>(0, max_index);
  auto index = distribution(random_device);
  return domain_[index];
}
}
