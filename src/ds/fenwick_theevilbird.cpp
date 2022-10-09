struct Fenwick {
    int n;
    vector<ll> f;

    Fenwick(int _n) {
        n = _n;
        f.assign(n + 1, 0);
    }

    void update(int x, ll delta) {
        for (int i = x; i <= n; i += i & -i) {
            f[i] += delta;
        }
    }

    ll get_sum(int x) {
        ll s = 0;
        for (int i = x; i > 0; i -= i & -i) {
            s += f[i];
        }
        return s;
    }

    ll get(int L, int R) {// [L, R]
        return get_sum(R) - get_sum(L - 1);
    }
};
