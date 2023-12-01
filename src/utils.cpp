#include "utils.hpp"

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
