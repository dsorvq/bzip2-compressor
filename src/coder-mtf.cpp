#include "coder-mtf.hpp"

void CoderMTF::encode(std::istream& input, std::ostream& output)  {
  std::list<uint8_t> alphabet(alphabet_size);
  fill_alphabet(alphabet);

  for (uint8_t ch; input.read(reinterpret_cast<char*>(&ch), sizeof ch); ) {
    uint8_t position = 0;
    auto symbol = begin(alphabet);

    while (ch != *symbol) {
      ++position;
      ++symbol;
    }

    alphabet.splice(begin(alphabet), alphabet, symbol);
    output << position;
  }
}

void CoderMTF::decode(std::istream& input, std::ostream& output) { 
  std::list<uint8_t> alphabet(alphabet_size);
  fill_alphabet(alphabet);
   
  for (uint8_t ch; input.read(reinterpret_cast<char*>(&ch), sizeof ch); ) {
    auto symbol = std::next(begin(alphabet), ch);
    output << *symbol;

    alphabet.splice(begin(alphabet), alphabet, symbol);
  }
}

void CoderMTF::fill_alphabet(std::list<uint8_t>& alphabet) {
  std::iota(begin(alphabet), end(alphabet), 0); 
}
