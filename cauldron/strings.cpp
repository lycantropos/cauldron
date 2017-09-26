#include "strings.h"


namespace strategies {
Strings::Strings(size_t length,
                 Characters alphabet,
                 unsigned max_attempts) :
    length_(length),
    alphabet_(std::move(alphabet)),
    max_attempts_(max_attempts) {}


Strings::Strings(size_t length,
                 Characters alphabet,
                 std::vector<utils::Predicate<std::string>> predicates,
                 unsigned int max_attempts) :
    length_(length),
    alphabet_(std::move(alphabet)),
    predicates_(std::move(predicates)),
    max_attempts_(max_attempts) {}


Strings::Strings(size_t length,
                 const char alphabet[]) :
    length_(length),
    alphabet_(Characters(alphabet)) {}


Strings::Strings(size_t length,
                 const std::string &alphabet) :
    length_(length),
    alphabet_(Characters(alphabet)) {}


Strings Strings::filter(const utils::Predicate<std::string> &predicate) const {
  auto predicates = std::vector<utils::Predicate<std::string>>(predicates_);
  predicates.push_back(predicate);
  return Strings(length_,
                 alphabet_,
                 predicates,
                 max_attempts_);
}


bool Strings::satisfactory(const std::string &string) const {
  return utils::object_satisfies_predicates<std::string>(string,
                                                         predicates_);
}


std::string Strings::operator()() const {
  for (unsigned _ = 0; _ < max_attempts_; ++_) {
    std::string result(length_, 0);
    std::generate_n(result.begin(),
                    length_,
                    alphabet_);
    if (not satisfactory(result)) {
      continue;
    }
    return result;
  }
  throw OutOfTries(max_attempts_);
}
}
