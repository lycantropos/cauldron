#include <iostream>
#include <cassert>
#include <cauldron/characters.h>


int main() {
  strategies::Characters characters("0123456789");

  assert(0 <= std::stoi(characters()) <= 9);

  std::cout << "Successfully installed \"cauldron\"." << std::endl;
}
