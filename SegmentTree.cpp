#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <string>

class SegmentTree {
 private:
  struct Node {
    int correct_count = 0;
    int open_count = 0;
    int closed_count = 0;
  };

  int size_ = 0;
  std::vector<Node> fund_nodes_;
  std::vector<Node> tree_;

  int LeftChild(int node) {
    return 2 * node + 1;
  }
  int RightChild(int node) {
    return 2 * node + 2;
  }
  int Parent(int node) {
    return (node - 1) / 2;
  }
  int Mid(int start, int end) {
    return (start + end) / 2;
  }

 public:
  explicit SegmentTree(std::string& data) {
    int log = static_cast<int>(std::log2(data.size()) + 1);
    size_ = 1 << log;
    data.resize(size_, '0');
    tree_.resize(2 * size_ - 1, {0, 0, 0});
    for (int i = size_ - 1; i < 2 * size_ - 1; ++i) {
      if (data[i - size_ + 1] == '(') {
        tree_[i].open_count = 1;
      } else if (data[i - size_ + 1] == ')') {
        tree_[i].closed_count = 1;
      }
    }
    for (int i = size_ - 2; i >= 0; --i) {
      int subseq_count = std::min(tree_[LeftChild(i)].open_count, tree_[RightChild(i)].closed_count);

      tree_[i].correct_count =
          tree_[LeftChild(i)].correct_count + tree_[RightChild(i)].correct_count + 2 * subseq_count;
      tree_[i].open_count = tree_[LeftChild(i)].open_count + tree_[RightChild(i)].open_count - subseq_count;
      tree_[i].closed_count = tree_[LeftChild(i)].closed_count + tree_[RightChild(i)].closed_count - subseq_count;
    }
  }

  int MaxSubseq(int l, int r) {
    fund_nodes_.clear();
    Query(l, r);
    if (fund_nodes_.size() == 1) {
      return fund_nodes_[0].correct_count;
    }
    Node node{0, 0, 0};
    for (int i = 0; i < static_cast<int>(fund_nodes_.size()); ++i) {
      int constant = std::min(node.open_count, fund_nodes_[i].closed_count);
      node.correct_count += fund_nodes_[i].correct_count + 2 * constant;
      node.open_count += fund_nodes_[i].open_count - constant;
      node.closed_count += fund_nodes_[i].closed_count - constant;
    }
    return node.correct_count;
  }

  void Query(int left, int right, int node = 0, int start = -1, int end = -1) {
    if (start == -1) {
      start = size_ - 1;
      end = 2 * size_ - 2;
      left += size_ - 1;
      right += size_ - 1;
    }
    if (start >= left && end <= right) {
      fund_nodes_.push_back(tree_[node]);
      return;
    }
    if (end < left || start > right) {
      return;
    }
    Query(left, right, LeftChild(node), start, (start + end) / 2);
    Query(left, right, RightChild(node), (start + end) / 2 + 1, end);
  }
};

void ProcessData() {
  int m = 0;
  std::string data;
  std::cin >> data;

  SegmentTree tree(data);
  std::cin >> m;
  for (int i = 0; i < m; ++i) {
    int l = 0;
    int r = 0;
    std::cin >> l >> r;
    std::cout << tree.MaxSubseq(l - 1, r - 1) << '\n';
  }
}

int main() {
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);
  ProcessData();
}
