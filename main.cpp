#include "include/coder.hpp"

int main() {
  auto coder = CoderMTF();  
  coder.encode(std::cin, std::cout);
  return 0;
}
