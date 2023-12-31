#include "bits.hpp"

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
  std::vector<char> number_chars(4);

  for (int i = 0; i < 4; ++i) {
    input.get(number_chars[i]);
  } 
  uint32_t result = 0;
  for (int i = 3; i >= 0; --i) {
    result |= (static_cast<unsigned char>(number_chars[3 - i]) << (8 * i));
  }
  return result;
}

BitWriter::~BitWriter() {
  if (buffer_index != 7) {
    flush();
  }
}

auto BitWriter::write(const std::vector<int>& bits) -> void {
  for (size_t i = 0; i < bits.size(); ++i) {
    buffer |= (bits[i] << buffer_index);
    if (buffer_index == 0) {
      flush();
    } else {
      --buffer_index;
    }
  }
}

auto BitWriter::flush() -> void {
  if (buffer_index != 7) {
    output << buffer;  
  }
  buffer_index = 7;
  buffer = 0;
}

auto BitReader::read() -> short {
  if (buffer_index == 7) {
    input.get(buffer);
  }

  short bit = (static_cast<unsigned char>(buffer) >> buffer_index) % 2;
  if (buffer_index) {
    --buffer_index;
  } else {
    buffer_index = 7;
  }
  return bit;
}

auto BitReader::flush() -> void {
  buffer_index = 7; 
}
