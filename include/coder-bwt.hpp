#pragma once

#include "coder.hpp"

class CoderBWT : public Coder {
public:
  CoderBWT() : CoderBWT(1) {}
  CoderBWT(int block_size) : block_size_{block_size} {}; 

  void encode(std::istream& input, std::ostream& output) override {};
  void decode(std::istream& input, std::ostream& output) override {};

private:
  int block_size_;
};
