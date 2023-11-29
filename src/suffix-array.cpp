#include "suffix-array.hpp"

SuffixArray::SuffixArray(std::string_view s) {
  // "aaba"
  // k = 0 -> p={0,1,3,2}, c={0,0,1,0}   (a, a, b, a)
  // k = 1 -> p={0,3,1,2}, c={0,1,2,0}   (aa, ab, ba, aa)
  // k = 2 -> p={3,0,1,2}, c={0,1,2,3}   (aaba, abaa, baaa, aaab)
  // p[i] <-> s[p[i] ... p[i] + 2^k - 1] is the i'th smallest 'circular suffix'
  // c[i] <-> 'equivalence class' of s[i ... i + 2^k - 1]
  int n = s.size();
  std::vector<int> c(n); 
  p.resize(n);

  std::vector<int> cnt(std::max(alphabet_size, n));
  for (auto ch : s) {
    cnt[ch]++;
  }
  std::inclusive_scan(begin(cnt), end(cnt), begin(cnt));
  for (int i = 0; i < n; ++i) {
    p[--cnt[s[i]]] = i;
  }

  c[p[0]] = 0;
  int number_of_classes = 1;
  for (int i = 1; i < n; ++i) {
    if (s[p[i - 1]] != s[p[i]]) {
      ++number_of_classes;
    }
    c[p[i]] = number_of_classes - 1;
  }

  std::vector<int> pn(n), cn(n);
  for (int h = 0; (1 << h) < n; ++h) {
    for (int i = 0; i < n; ++i) {
      pn[i] = p[i] - (1 << h);
      if (pn[i] < 0)
        pn[i] += n;
    }

    std::fill(begin(cnt), begin(cnt) + number_of_classes, 0);
    for (int i = 0; i < n; ++i) {
      ++cnt[c[pn[i]]];
    }
    std::inclusive_scan(begin(cnt), begin(cnt) + number_of_classes, begin(cnt));
    for (int i = n - 1; i >= 0; --i) {
      p[--cnt[c[pn[i]]]] = pn[i];
    }

    cn[p[0]] = 0;
    number_of_classes = 1;
    for (int i = 1; i < n; i++) {
      std::pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
      std::pair<int, int> prev = {c[p[i-1]], c[(p[i-1] + (1 << h)) % n]};
      if (cur != prev)
        ++number_of_classes;

      cn[p[i]] = number_of_classes - 1;
    }
    c.swap(cn);
  }
}
