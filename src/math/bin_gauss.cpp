int gauss(vector<bitset<N>> a, int n, int m,
          bitset<N> &ans) {
    vector<int> where(m, -1);
    for (int col = 0, row = 0; col < m && row < n; ++col) {
        for (int i = row; i < n; ++i)
            if (a[i][col]) {
                swap(a[i], a[row]);
                break;
            }
        if (!a[row][col]) {
            continue;
        }
        where[col] = row;
        for (int i = 0; i < n; ++i) {
            if (i != row && a[i][col]) a[i] ^= a[row];
        }
        ++row;
    }
}