#pragma once

#include "coder.hpp"

class CoderMTF : public Coder {
public:
  CoderMTF() {}; 

  void encode(std::istream& input, std::ostream& output) override {
    for (uint8_t ch; input.read(reinterpret_cast<char*>(&ch), sizeof ch); ) {
      output << ch;
    }
  };
  void decode(std::istream& input, std::ostream& output) override {};
};
