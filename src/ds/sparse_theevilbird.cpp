// usage:
//   auto fun = [&](int i, int j) { return min(i, j); };
//   SparseTable<int, decltype(fun)> st(a, fun);
// or:
//   SparseTable<int> st(a, [&](int i, int j) { return min(i, j); });
template <typename T, class F = function<T(const T&, const T&)>>
class SparseTable {
 public:
  int n;
  vector<int> lg;
  vector<vector<T>> mat;
  F func;

  SparseTable(const vector<T>& a, const F& f) : func(f) {
    n = static_cast<int>(a.size());
    lg.resize(n + 1);
    for (int i = 2; i <= n; ++i) {
      lg[i] = lg[i / 2] + 1;
    }
    mat.resize(lg[n] + 1);
    mat[0] = a;
    for (int j = 1; j <= lg[n]; j++) {
      mat[j].resize(n - (1 << j) + 1);
      for (int i = 0; i <= n - (1 << j); i++) {
        mat[j][i] = func(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
      }
    }
  }

  T get(int from, int to) const {
    assert(0 <= from && from <= to && to <= n - 1);
    int l = lg[to - from + 1];
    return func(mat[l][from], mat[l][to - (1 << l) + 1]);
  }
};