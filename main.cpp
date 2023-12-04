#include <gtest/gtest.h>
#include <random>
#include <fstream>

#include "coder-huffman.hpp"
#include "include/coder.hpp"
#include "include/coder-mtf.hpp"
#include "include/coder-bwt.hpp"
#include "bits.hpp"

auto generate_random_string(int size) -> std::string {
  std::string characters = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<> distribution(0, characters.size() - 1);

  std::string random_string;
  for (int i = 0; i < size; ++i) {
      random_string += characters[distribution(generator)];
  }

  return random_string;
}

auto encode_decode(Coder& coder, const std::string& original_string) -> std::string {
  std::stringstream input(original_string);
  std::stringstream output;

  coder.encode(input, output);

  input.clear(); 
  input.str("");
  output.seekg(0); 

  coder.decode(output, input);

  std::string resulting_string;
  std::getline(input, resulting_string); 
  
  return resulting_string;
}

auto encode(Coder& coder, std::string& original_string) -> std::string {
  std::stringstream input(original_string);
  std::stringstream output;

  coder.encode(input, output);

  std::string resulting_string;
  std::getline(output, resulting_string); 
  
  return resulting_string;
}

TEST(CoderMTF_test, small_random_string) {
  int size = 10;
  std::string original_string = generate_random_string(size);
  CoderMTF coder;

  EXPECT_EQ(original_string, encode_decode(coder, original_string));
}

TEST(CoderMTF_test, large_random_string) {
  int size = 100000;
  std::string original_string = generate_random_string(size);
  CoderMTF coder;

  EXPECT_EQ(original_string, encode_decode(coder, original_string));
}

TEST(CoderBWT_test, banana_string) {
  std::string original_string = "banana";
  CoderBWT coder;
  EXPECT_EQ(original_string, encode_decode(coder, original_string));
}

TEST(CoderBWT_test, small_random_string) {
  int size = 10;
  std::string original_string = generate_random_string(size);
  CoderBWT coder;

  EXPECT_EQ(original_string, encode_decode(coder, original_string));
}

TEST(CoderBWT_test, large_random_string) {
  int size = 10000;
  std::string original_string = generate_random_string(size);
  CoderBWT coder;

  EXPECT_EQ(original_string, encode_decode(coder, original_string));
}

TEST(CoderHuffman_test, small_random_string) {
  int size = 10;
  std::string original_string = generate_random_string(size);
  CoderHuffman coder;

  EXPECT_EQ(original_string, encode_decode(coder, original_string));
}

TEST(CoderHuffman_test, large_random_string) {
  int size = 10000;
  std::string original_string = generate_random_string(size);
  CoderHuffman coder;

  EXPECT_EQ(original_string, encode_decode(coder, original_string));
}

auto main(int argc, char* argv[]) -> int {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
