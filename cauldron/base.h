#pragma once

namespace strategies {
static const unsigned MAX_ATTEMPTS = 1000;


class OutOfTries : public std::exception {
 public:
  explicit OutOfTries(unsigned max_tries) :
      msg_("Could not generate any valid examples in "
               + std::to_string(max_tries)
               + " attempts") {};

  const char *what() const throw() {
    return msg_.c_str();
  }

 private:
  std::string msg_;
};


template<typename T>
class Generator {
  virtual T operator()() const = 0;
};
}
