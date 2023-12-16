#include <iostream>
#include <numeric>
#include <vector>

template <class T>
class FenwickTree {
 private:
  std::vector<T> fenwick_;

 public:
  explicit FenwickTree(const std::vector<T>& array) : fenwick_(array.size(), 0) {
    int64_t n = array.size();
    std::vector<T> prefix_sums(n + 1, 0);

    for (int64_t i = 1; i <= n; ++i) {
      prefix_sums[i] = prefix_sums[i - 1] + array[i - 1];
    }
    for (int64_t i = 0; i < n; ++i) {
      if ((i & (i + 1)) >= 0) {
        fenwick_[i] = prefix_sums[i + 1] - prefix_sums[(i & (i + 1))];
      } else {
        fenwick_[i] = prefix_sums[i + 1];
      }
    }
  }

  T Query(int64_t right) {
    int64_t sum = 0;
    for (int64_t i = right; i >= 0; i = (i & (i + 1)) - 1) {
      sum += fenwick_[i];
    }
    return sum;
  }

  T Query(int64_t left, int64_t right) {
    if (left - 1 < 0) {
      return Query(right);
    }
    return Query(right) - Query(left - 1);
  }

  void Update(int64_t index, T delta) {
    for (size_t i = index; i < fenwick_.size(); i = i | (i + 1)) {
      fenwick_[i] += delta;
    }
  }
};

void Solve() {
  int64_t n = 0;
  int64_t m = 0;
  std::cin >> n;
  std::vector<int64_t> numbers(n);
  for (int64_t i = 0; i < n; ++i) {
    std::cin >> numbers[i];
  }
  FenwickTree tree(numbers);
  std::cin >> m;
  int64_t left = 0;
  int64_t right = 0;
  int64_t index = 0;
  int64_t value = 0;
  char sym = '0';
  for (int64_t i = 0; i < m; ++i) {
    std::cin >> sym;
    if (sym == 's') {
      std::cin >> left >> right;
      std::cout << tree.Query(left - 1, right - 1) << ' ';
    } else {
      std::cin >> index >> value;
      tree.Update(index - 1, value - numbers[index - 1]);
      numbers[index - 1] = value;
    }
  }
}

int main() {
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);
  Solve();
}
