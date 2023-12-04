#pragma once

#include <ostream>
#include <istream>
#include <vector>

void put_uint32_t(uint32_t number, std::ostream& output);

auto read_uint32_t(std::istream& input) -> uint32_t;

class BitWriter {
public:
  BitWriter(std::ostream& output_stream) : buffer {0}, buffer_index {7}, output{output_stream} { }
  ~BitWriter();

  auto write(const std::vector<int>& bits) -> void;

  auto flush() -> void;

private:
  uint8_t buffer;
  size_t buffer_index;
  std::ostream& output;
};

class BitReader {
public:
  BitReader(std::istream& intput_stream) : buffer {0}, buffer_index {7}, input{intput_stream} { };

  // actually reads by bytes so overhead bits are wasted if not used by this
  auto read() -> short;

  auto flush() -> void;

private:
  char buffer;
  size_t buffer_index;
  std::istream& input;
};
