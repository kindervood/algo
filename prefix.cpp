void Prefix(const std::string& s, std::vector<int>& prefix) {
  int n = static_cast<int>(s.length());
  prefix.resize(n, 0);

  for (int i = 1; i < n; i++) {
    int j = prefix[i - 1];
    while (j > 0 && s[i] != s[j]) {
      j = prefix[j - 1];
    }
    if (s[i] == s[j]) {
      j++;
    }
    prefix[i] = j;
  }
}
