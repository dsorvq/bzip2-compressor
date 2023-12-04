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

  print_huffman_tree_symbols(huffman_tree_root, output);

  // TODO: delete the tree
  
  // write encoded text
  for (auto ch : input_string) {
    bw.write(code_table[ch]);
  }

  delete_tree(huffman_tree_root);
}

void CoderHuffman::decode(std::istream &input, std::ostream &output) {
  auto text_size = read_uint32_t(input); 
  auto flat_tree_size = read_uint32_t(input);

  std::string flat_tree;
  auto br = BitReader(input);
  for (size_t i = 0; i < flat_tree_size; ++i) { 
    auto bit = br.read();
    flat_tree.push_back(bit + '0');  
  }
  br.flush();

  auto* huffman_tree_root = unflatten_huffman_tree(flat_tree, input);

  auto* huffman_node = huffman_tree_root; 
  for (size_t decoded_symbols = 0; decoded_symbols < text_size; ) {
    if (!huffman_node->zero) {
      output << huffman_node->symbol;
      huffman_node = huffman_tree_root;
      ++decoded_symbols;
    } else {
      auto direction = br.read();
      if (direction == 0) {
        huffman_node = huffman_node->zero;
      } else {
        huffman_node = huffman_node->one;
      }
    }
  }

  delete_tree(huffman_tree_root);
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
    //code_table.push_back(current_code);
    code_table[root->symbol] = current_code;
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

auto CoderHuffman::print_huffman_tree_symbols(HuffmanNode* root, std::ostream& output) -> void {
  if (!root->zero) {
    output << root->symbol;
    return;
  }
  print_huffman_tree_symbols(root->zero, output);
  print_huffman_tree_symbols(root->one, output);
}

auto unflatten_huffman_tree_dfs(std::string_view flat_tree, size_t& flat_tree_i, HuffmanNode* node, std::istream& input) -> void {
  if (flat_tree_i == flat_tree.size()) {
    input.get(node->symbol);
    return;
  }

  if (flat_tree[flat_tree_i] == '0') {
    node->zero = new HuffmanNode(static_cast<char>(0), 0);
    ++flat_tree_i;
    unflatten_huffman_tree_dfs(flat_tree, flat_tree_i, node->zero, input);
  } 

  if (flat_tree_i == flat_tree.size()) {
    return;
  }
    
  if (flat_tree[flat_tree_i] == '1') {
    if (node->zero) {
      ++flat_tree_i;
      node->one = new HuffmanNode(static_cast<char>(0), 0);
      unflatten_huffman_tree_dfs(flat_tree, flat_tree_i, node->one, input);
    } else {
      input.get(node->symbol);
    }
  }
}

auto CoderHuffman::unflatten_huffman_tree(std::string_view flat_tree, std::istream& input) -> HuffmanNode* {
  auto* root = new HuffmanNode(static_cast<char>(0), 0);
  size_t id = 0;
  unflatten_huffman_tree_dfs(flat_tree, id, root, input);
  
  return root;
}

auto CoderHuffman::delete_tree(HuffmanNode* root) -> void {
  if (!root) {
    return; 
  }
  if (root->zero) {
    delete_tree(root->zero); 
    delete_tree(root->one); 
  }
  delete root;
}
