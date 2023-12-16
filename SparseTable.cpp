#include <iostream>
#include <vector>
#include <numeric>

int log_2(int x) {
  int bit = (1 << 31);
  int result = 31;
  while ((x & bit) == 0) {
    bit >>= 1;
    --result;
  }
  return result;
}

template<class T>
class SparseTable {
 private:
  std::vector<std::vector<T>> st_;
 public:
  SparseTable(const std::vector<T>& array) {
    int n = array.size();
    int max_k = log_2(n) + 1;
    st_.resize(max_k, std::vector<T>(n));
    for (int i = 0; i < n; ++i) {
      st_[0][i] = array[i];
    }
    for (int k = 1; k <= max_k; ++k) {
      for (int i = 0; i + (1 << k) <= n; ++i) {
        st_[k][i] = std::gcd(st_[k - 1][i], st_[k - 1][i + (1 << (k - 1))]);
      }
    }
  }

  T query(int64_t left, int64_t right) {
    int k = log_2(right - left);
    return std::gcd(st_[k][left], st_[k][right - (1 << k)]);
  }

  void print() {
    int n = st_[0].size();
    int max_k = st_.size();
    for (int k = 0; k <= max_k; ++k) {
      for (int i = 0; i + (1 << k) <= n; ++i) {
        std::cout << st_[k][i] << ' ';
      }
      std::cout << '\n';
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  int64_t n, q;
  std::cin >> n;
  std::vector<int64_t> numbers(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> numbers[i];
  }
  std::cin >> q;
  SparseTable st(numbers);
  for (int i = 0; i < q; ++i) {
    int64_t left, right;
    std::cin >> left >> right;
    std::cout << st.query(left - 1, right) << '\n';
  }
}
