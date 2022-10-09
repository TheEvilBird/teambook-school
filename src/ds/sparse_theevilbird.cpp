struct Sparse {
    int n;
    vector<int> lg;
    vector<vector<ll>> table;

    Sparse(int _n) {
        n = _n;
        lg.resize(n + 1);
        for (int i = 2; i <= n; ++i) {
            lg[i] = lg[i / 2] + 1;
        }
        table.resize(lg[n] + 1, vector<ll>(n));
        for (int i = 0; i < n; ++i) table[0][i] = INF;
    }

    void build() {
        for (int i = 1; i < table.size(); ++i) {
            for (int j = 0; j + (1 << i) <= n; ++j) {
                table[i][j] = min(table[i - 1][j],
                                  table[i - 1][j + (1 << (i - 1))]);
            }
        }
    }

    ll get(int L, int R) {// [L, R]
        int D = R - L + 1, x = lg[D];
        return min(table[x][L], table[x][R - (1 << x) + 1]);
    }
};