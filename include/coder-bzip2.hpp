#pragma once

#include "coder-bwt.hpp"
#include "coder-mtf.hpp"
#include "coder-huffman.hpp"

class CoderB2zip : public Coder {
public:
  CoderB2zip() : CoderB2zip(1) {};
  CoderB2zip(int bwt_block_size) : bwt_block_size_{bwt_block_size} {}

  void encode(std::istream& input, std::ostream& output) override {
    auto mtf = CoderMTF();
    auto bwt = CoderBWT(bwt_block_size_);
    auto hfm = CoderHuffman();

    std::stringstream temporary_data1, temporary_data2;

    mtf.encode(input, temporary_data1);
    bwt.encode(temporary_data1, temporary_data2);
    hfm.encode(temporary_data2, output);
  };

  void decode(std::istream& input, std::ostream& output) override {};

private:
  int bwt_block_size_;
};
