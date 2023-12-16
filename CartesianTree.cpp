#include <climits>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

class CartesianTree {
 private:
  struct Node {
    int64_t key;
    int64_t priority;
    int64_t sum = 0;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;

    Node(int64_t k, int64_t p) : key(k), priority(p), sum(k) {
    }
  };

  Node* root_ = nullptr;

  int64_t Sum(Node* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->sum;
  }

  void UpdateSum(Node* node) {
    if (node == nullptr) {
      return;
    }
    node->sum = Sum(node->left) + Sum(node->right) + node->key;
  }

  Node* Merge(Node* first, Node* second) {
    if (first == nullptr) {
      return second;
    }
    if (second == nullptr) {
      return first;
    }

    if (first->priority < second->priority) {
      first->right = Merge(first->right, second);
      first->right->parent = first;
      UpdateSum(first);
      return first;
    }
    second->left = Merge(first, second->left);
    second->left->parent = second;
    UpdateSum(second);
    return second;
  }

  std::pair<Node*, Node*> Split(Node* tree, int64_t key) {
    if (!tree) {
      return {nullptr, nullptr};
    }
    if (tree->key < key) {
      auto [Less, Geq] = Split(tree->right, key);
      tree->right = Less;
      if (Less) {
        Less->parent = tree;
      }
      UpdateSum(tree);
      return {tree, Geq};
    }
    auto [Less, Geq] = Split(tree->left, key);
    tree->left = Geq;
    if (Geq) {
      Geq->parent = tree;
    }
    UpdateSum(tree);
    return {Less, tree};
  }

 public:
  void Insert(int64_t x, int64_t y) {
    auto new_node = new Node(x, y);
    if (root_) {
      auto [Less, Geq] = Split(root_, x);
      root_ = Merge(Merge(Less, new_node), Geq);
    } else {
      root_ = new_node;
    }
  }

  bool Exists(int64_t x) {
    Node* cur = root_;
    while (cur != nullptr) {
      if (cur->key == x) {
        return true;
      }
      if (cur->key < x) {
        cur = cur->right;
      } else {
        cur = cur->left;
      }
    }
    return false;
  }

  int64_t Sum(int64_t l, int64_t r) {
    auto [Less, Geq] = Split(root_, l);
    auto [Eq, Greater] = Split(Geq, r + 1);
    auto ans = Sum(Eq);
    root_ = Merge(Less, Merge(Eq, Greater));
    return ans;
  }

  void Delete(Node* node) {
    if (node->left) {
      Delete(node->left);
    }
    if (node->right) {
      Delete(node->right);
    }
    delete node;
  }

  void Delete() {
    Delete(root_);
  }
};

int main() {
  CartesianTree tree;

  int64_t n = 0;
  std::cin >> n;
  for (int64_t i = 0; i < n; ++i) {
    char command = '0';
    std::cin >> command;
    if (command == '+') {
      int64_t x = 0;
      std::cin >> x;
      if (!tree.Exists(x)) {
        tree.Insert(x, std::rand());
      }
    } else {
      int64_t left = 0;
      int64_t right = 0;
      std::cin >> left >> right;
      std::cout << tree.Sum(left, right) << '\n';
    }
  }
  tree.Delete();
}
