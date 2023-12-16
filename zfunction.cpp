void ZFunction(const std::string& s, std::vector<int>& z_func) {
  int n = static_cast<int>(s.length());
  z_func.resize(n, 0);

  int left = 0;
  int right = 0;
  for (int i = 1; i < n; i++) {
    if (i < right) {
      z_func[i] = std::min(right - i, z_func[i - left]);
    }
    while (i + z_func[i] < n && s[z_func[i]] == s[i + z_func[i]]) {
      ++z_func[i];
    }
    if (i + z_func[i] > right) {
      left = i;
      right = i + z_func[i];
    }
  }
  z_func[0] = n;
}
