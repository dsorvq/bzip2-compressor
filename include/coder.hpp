#pragma once 

#include <iostream>
#include <sstream>

class Coder {
public:
  virtual void encode(std::istream& input, std::ostream& output) = 0;
  virtual void decode(std::istream& input, std::ostream& output) = 0;
};
