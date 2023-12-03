#pragma once

#include "coder.hpp"
#include "bits.hpp"
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <queue>

struct HuffmanNode {
  HuffmanNode* zero; 
  HuffmanNode* one; 
  char symbol;
  size_t freq;

  HuffmanNode(char symbol, size_t frequency) : symbol{symbol} {
    zero = one = nullptr;
  }
  ~HuffmanNode() {
    if (zero)
      delete zero;
    if (one)
      delete one;
  }

  HuffmanNode(HuffmanNode* zero, HuffmanNode* one) : zero {zero}, one {one} {
    freq = zero->freq + one->freq;
  }
};

class CoderHuffman : public Coder {
public:
  CoderHuffman() {}

  void encode(std::istream& input, std::ostream& output) override;
  void decode(std::istream& input, std::ostream& output) override;

private:
  auto build_symbol_frequency_table(std::string_view input_symbols) -> std::vector<size_t>;
  auto build_huffman_tree(std::vector<size_t>& frequency_table) -> HuffmanNode*;  
  auto build_code_table(HuffmanNode* root) -> std::vector<std::vector<int>>;
  auto flatten_huffman_tree(HuffmanNode* root) -> std::string;

private:
  int const alphabet_size {256};
};
