#include "strings.h"


namespace strategies {
Strings::Strings(Generator<size_t> *lengths,
                 Generator<char> *alphabet,
                 unsigned max_attempts) :
    lengths_(lengths),
    alphabet_(alphabet),
    max_attempts_(max_attempts) {}


Strings::Strings(Generator<size_t> *lengths,
                 Generator<char> *alphabet,
                 const std::vector<utils::Predicate<std::string>> &predicates,
                 unsigned int max_attempts) :
    lengths_(lengths),
    alphabet_(alphabet),
    predicates_(predicates),
    max_attempts_(max_attempts) {}


Strings::Strings(Generator<size_t> *lengths,
                 const char *alphabet) :
    lengths_(lengths),
    alphabet_(new Characters(alphabet)) {}


Strings::Strings(Generator<size_t> *lengths,
                 const std::string &alphabet) :
    lengths_(lengths),
    alphabet_(new Characters(alphabet)) {}


Strings Strings::filter(const utils::Predicate<std::string> &predicate) const {
  auto predicates = std::vector<utils::Predicate<std::string>>(predicates_);
  predicates.push_back(predicate);
  return Strings(lengths_,
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
    auto length = (*lengths_)();
    std::string result(length, 0);
    std::generate_n(result.begin(),
                    length,
                    [&]() -> char {
                      return (*alphabet_)();
                    });
    if (not satisfactory(result)) {
      continue;
    }
    return result;
  }
  throw OutOfTries(max_attempts_);
}
}
