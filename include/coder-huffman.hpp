#pragma once

#include "coder.hpp"

class CoderHuffman : public Coder {
public:
  CoderHuffman() {}

  void encode(std::istream& input, std::ostream& output) override {};
  void decode(std::istream& input, std::ostream& output) override {};
};
