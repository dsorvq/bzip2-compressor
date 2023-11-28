#pragma once

#include <list>
#include <numeric>

#include "coder.hpp"

class CoderMTF : public Coder {
public:
  CoderMTF() {}; 

  void encode(std::istream& input, std::ostream& output) override;
  void decode(std::istream& input, std::ostream& output) override;

private:
  void fill_alphabet(std::list<uint8_t>& alphabet);
  const int alphabet_size = 256;
};
