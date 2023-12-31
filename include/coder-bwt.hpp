#pragma once

#include "coder.hpp"
#include "bits.hpp"
#include "suffix-array.hpp"

class CoderBWT : public Coder {
public:
  CoderBWT() {}; //: CoderBWT(1) {}
  //CoderBWT(int block_size) : block_size_{block_size} {}; 

  void encode(std::istream& input, std::ostream& output) override;
  void decode(std::istream& input, std::ostream& output) override;

private:
  int block_size_;
  int alphabet_size = 256;
};
