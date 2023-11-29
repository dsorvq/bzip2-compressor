#include <vector>
#include <algorithm>
#include <string>
#include <string_view>
#include <numeric>

class SuffixArray {
public:
  SuffixArray(std::string_view s);

  auto operator[](int i) const -> int {
    return p[i]; 
  }

private:
  std::vector<int> p;
  int const alphabet_size = 256;
};

