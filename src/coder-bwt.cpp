#include "coder-bwt.hpp"

void put_uint32_t(uint32_t number, std::ostream& output) {
  uint64_t mask1 = (1 << 8) - 1;
  uint32_t mask = mask1 << (3 * 8);
  for (int i = 3; i >= 0; i--) {
    uint32_t current_byte = (mask & number) >> (i * 8);
    output << (uint8_t) current_byte;
    mask >>= 8;
  }
}

auto read_uint32_t(std::istream& input) -> uint32_t {
  std::vector<uint8_t> number_chars(4);

  for (int i = 0; i < 4; ++i) {
    input >> number_chars[i];
  } 
  uint32_t result = 0;
  for (int i = 3; i >= 0; --i) {
    result |= (number_chars[3 - i] << (8 * i));
  }
  return result;
}

void CoderBWT::encode(std::istream& input, std::ostream& output) {
  std::string input_string((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

  input_string += input_string;// + '\0'; 
  auto suffix_arr = SuffixArray(input_string);
  auto n = input_string.size();

  for (size_t i = 0; i < n; i++) {
    if (suffix_arr[i] == 0) {
      put_uint32_t(i / 2, output);
      break;
    }
    //std::cout << i << ':' << suffix_arr[i] << '\n';
  }
  //put_uint32_t(3, output);

  for (int i = 0; i < n; ++i) {
    if (suffix_arr[i] < n / 2) {
      output << input_string[suffix_arr[i] + n / 2 - 1];
    }
  }
}

//void BWT::decode(std::istream &input, std::ostream &output)
void CoderBWT::decode(std::istream& input, std::ostream& output) {
    if (input.rdbuf()->in_avail()==0) {
        output << "";

        return;
    }
    uint32_t number = read_uint32_t(input);
    std::vector<uint8_t> input_chars((std::istreambuf_iterator<char>(input)),
                                     std::istreambuf_iterator<char>());
    std::vector<size_t> chars_counter(256, 0);
    for (size_t i = 0; i < input_chars.size(); i++) {
        ++chars_counter[input_chars[i]];
    }
    size_t sum = 0;
    for (size_t i = 0; i < chars_counter.size(); i++) {
        sum += chars_counter[i];
        chars_counter[i] = sum - chars_counter[i];
    }
    std::vector<size_t> reverse_indexes(input_chars.size());

    for (size_t i = 0; i < input_chars.size(); i++) {
        reverse_indexes[chars_counter[input_chars[i]]] = i;
        ++chars_counter[input_chars[i]];
    }
    size_t j = reverse_indexes[number];
    for (size_t i = 0; i < input_chars.size(); i++) {
        output << input_chars[j];

        j = reverse_indexes[j];
    }
}

/*
void CoderBWT::decode(std::istream& input, std::ostream& output) {
  auto number = read_uint32_t(input);
  std::string input_string((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

  std::vector<size_t> symbol_counters(alphabet_size, 0);
  for (size_t i = 0; i < input_string.size(); ++i) {
    ++symbol_counters[input_string[i]];
  }
  size_t total = 0;
  for (size_t i = 0; i < symbol_counters.size(); i++) {
    total += symbol_counters[i];
    symbol_counters[i] = total - symbol_counters[i];
  }

  std::vector<size_t> reverse_indexes(input_string.size());
  for (size_t i = 0; i < input_string.size(); i++) {
    reverse_indexes[symbol_counters[input_string[i]]] = i;
    ++symbol_counters[input_string[i]];
  }

  for (size_t i = 0, j = reverse_indexes[number]; i < input_string.size(); i++) {
    output << input_string[j];
    j = reverse_indexes[j];
  } 
} */
