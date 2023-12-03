#include "coder-huffman.hpp"

void CoderHuffman::encode(std::istream& input, std::ostream& output) {
  std::string input_string((std::istreambuf_iterator<char>(input)), {});
  auto frequency_table = build_symbol_frequency_table(input_string);
  auto* huffman_tree_root = build_huffman_tree(frequency_table);
  auto code_table = build_code_table(huffman_tree_root);
  auto flat_tree = flatten_huffman_tree(huffman_tree_root);
  
  // encoded text:
  //   8 bytes = number of characters is original text
  //   8 bytes = number of bytes in the representation of the flatten tree 
  //   characters in postorder traversal of the tree
  //   encoded original text
  put_uint32_t(static_cast<uint32_t>(input_string.size()), output);
  put_uint32_t(static_cast<uint32_t>(flat_tree.size()), output);

  // write tree
  BitWriter bw(output);
  for (auto bit : flat_tree) {
    bw.write({bit - '0'}); 
  }
  bw.flush();

  // write encoded text
  for (auto ch : input_string) {
    bw.write(code_table[ch]); 
  }
}

void CoderHuffman::decode(std::istream &input, std::ostream &output) {
  
}

auto CoderHuffman::build_symbol_frequency_table(std::string_view intput_symbols) -> std::vector<size_t> {
  std::vector<size_t> frequency_table(alphabet_size);
   
  for (auto c : intput_symbols) {
    ++frequency_table[c];
  }

  return frequency_table;
}

auto CoderHuffman::build_huffman_tree(std::vector<size_t>& frequency_table) -> HuffmanNode* {
  struct comp {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
      return a->freq > b->freq; 
    }
  };

  std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, comp> huffman_queue;
 
  for (size_t i = 0; i < frequency_table.size(); ++i) {
    if (frequency_table[i]) {
      auto* node = new HuffmanNode(i, frequency_table[i]);
      huffman_queue.push(node);
    }
  }

  while (huffman_queue.size() > 1) {
    auto a = huffman_queue.top(); 
    huffman_queue.pop();
    auto b = huffman_queue.top(); 
    huffman_queue.pop();
    
    huffman_queue.push(new HuffmanNode(a, b));
  }

  return huffman_queue.top();
}

void dfs(HuffmanNode* root, std::vector<int>& current_code, std::vector<std::vector<int>>& code_table) {
  if (root->zero) {
    current_code.push_back(0);
    dfs(root->zero, current_code, code_table);
    current_code.back() = 1;
    dfs(root->one, current_code, code_table);
    current_code.pop_back();
  } else {
    code_table.push_back(current_code);
  }
}

auto CoderHuffman::build_code_table(HuffmanNode* root) -> std::vector<std::vector<int>> {
  std::vector<std::vector<int>> code_table(alphabet_size);  

  std::vector<int> current_code;
  dfs(root, current_code, code_table);  

  return code_table;
}

auto flatten_huffman_tree_dfs(HuffmanNode* node, std::string& message) -> void {
  if (node->zero) {
    message += '0';
    flatten_huffman_tree_dfs(node->zero, message);
    message += '1';
    flatten_huffman_tree_dfs(node->one, message);
  }
}

auto CoderHuffman::flatten_huffman_tree(HuffmanNode* root) -> std::string {
  std::string flat_tree = "";
  flatten_huffman_tree_dfs(root, flat_tree);
  return flat_tree;
}
