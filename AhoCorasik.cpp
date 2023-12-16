#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>

struct Node {
  std::unordered_map<char, Node*> next;
  bool is_terminal = false;
  Node* exit_link = nullptr;
  Node* suff_ref = nullptr;
  int terminal_index = -1;
};

struct SuffixQueueNode {
  char sym;
  Node* parent = nullptr;
  Node* node = nullptr;

  SuffixQueueNode(char sym, Node* parent, Node* node) : sym(sym), parent(parent), node(node) {
  }
};

class Trie {
 public:
  Node* root = nullptr;

  Trie() {
    root = new Node();
  }

  explicit Trie(const std::vector<std::string>& patterns) {
    root = new Node();
    int index = 0;
    for (auto& s : patterns) {
      Insert(s, index++);
    }
  }

  bool Find(const std::string& str) {
    auto node = root;
    for (auto sym : str) {
      Node* next_node = nullptr;
      if (node->next.contains(sym)) {
        next_node = node->next[sym];
      }
      if (!next_node) {
        return false;
      }
      node = next_node;
    }
    return node->is_terminal;
  }

  void Insert(const std::string& str, int index) {
    auto node = root;
    for (auto sym : str) {
      Node* next_node = nullptr;
      if (node->next.contains(sym)) {
        next_node = node->next[sym];
      }
      if (!next_node) {
        auto new_node = new Node;
        node->next[sym] = new_node;
        next_node = new_node;
      }
      node = next_node;
    }
    node->is_terminal = true;
    node->terminal_index = index;
  }

  void BuildSuffixRef() {
    root->suff_ref = nullptr;
    std::queue<SuffixQueueNode> q;
    for (auto [sym, node] : root->next) {
      q.emplace(sym, root, node);
    }
    while (!q.empty()) {
      auto [s, parent, node] = q.front();
      q.pop();
      Node* suff = parent->suff_ref;
      while (suff != nullptr && !suff->next.contains(s)) {
        suff = suff->suff_ref;
      }
      node->suff_ref = (suff == nullptr ? root : suff->next[s]);
      for (auto [sym, neighbour] : node->next) {
        q.emplace(sym, node, neighbour);
      }
    }
  }

  void BulidExitLinks() {
    root->exit_link = nullptr;
    std::queue<Node*> q;

    for (auto& [sym, node] : root->next) {
      q.push(node);
    }
    while (!q.empty()) {
      Node* node = q.front();
      q.pop();
      Node* suff = node->suff_ref;
      if (suff != root) {
        node->exit_link = (suff->is_terminal ? suff : suff->exit_link);
      }
      for (auto [sym, neighbour] : node->next) {
        q.push(neighbour);
      }
    }
  }

  Node* NextState(Node* node, char sym) {
    while (node != nullptr && !node->next.contains(sym)) {
      node = node->suff_ref;
    }
    return (node == nullptr ? root : node->next[sym]);
  }

  void Delete(Node* node) {
    for (auto& [sym, neighbour] : node->next) {
      Delete(neighbour);
    }
    delete node;
  }
};

void CountEntries(int index, Node* node, std::vector<std::vector<int>>& counter, std::vector<std::string>& patterns) {
  if (!node->is_terminal) {
    node = node->exit_link;
  }
  while (node != nullptr) {
    int i = node->terminal_index;
    if (i >= 0) {
      counter[i].push_back(index + 2 - static_cast<int>(patterns[i].length()));
    }
    node = node->exit_link;
  }
}

void AhoCorasik(std::string& str, std::vector<std::string>& patterns) {
  Trie trie(patterns);
  std::vector<std::vector<int>> counter(patterns.size());
  trie.BuildSuffixRef();
  trie.BulidExitLinks();
  auto node = trie.root;
  for (int i = 0; i < static_cast<int>(str.length()); ++i) {
    node = trie.NextState(node, str[i]);
    CountEntries(i, node, counter, patterns);
  }
  for (int i = 0; i < static_cast<int>(counter.size()); ++i) {
    std::cout << counter[i].size() << ' ';
    for (int j = 0; j < static_cast<int>(counter[i].size()); ++j) {
      std::cout << counter[i][j] << ' ';
    }
    std::cout << '\n';
  }
  trie.Delete(trie.root);
}

int main() {
  std::string s;
  std::cin >> s;
  int n = 0;
  std::cin >> n;
  std::vector<std::string> patterns(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> patterns[i];
  }
  AhoCorasik(s, patterns);
}
