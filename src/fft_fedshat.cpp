using ll = long long;

constexpr int mod = 998244353, root = 3;

int binpow(int a, int n, int mod) {
    if (n == 0) {
        return 1;
    }
    if (n % 2 == 0) {
        int b = binpow(a, n / 2, mod);
        return (b * 1ll * b) % mod;
    }
    return (a * 1ll * binpow(a, n - 1, mod)) % mod;
}

vector<int> W1, W2;

vector<int> ntt(vector<int> a, bool inv = false) {
    if (a.size() == 1) {
        return a;
    }
    int n = a.size();
    vector<int> a0, a1;
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            a0.push_back(a[i]);
        } else {
            a1.push_back(a[i]);
        }
    }
    a0 = ntt(a0, inv);
    a1 = ntt(a1, inv);
    int w = 1, wn = W1[n];
    if (inv) {
        wn = W2[n];
    }
    vector<int> res(n);
    for (int k = 0; k < n / 2; ++k) {
        int t = (w * 1ll * a1[k]) % mod;
        res[k] = (a0[k] + t) % mod;
        res[k + n / 2] = ((a0[k] - t) % mod + mod) % mod;
        w = (w * 1ll * wn) % mod;
    }
    return res;
}

vector<int> ntt_inv(vector<int> a) {
    a = ntt(a, true);
    int n = a.size(), inv = binpow(n, mod - 2, mod);
    for (int i = 0; i < n; ++i) {
        a[i] = (a[i] * 1ll * inv) % mod;
    }
    return a;
}

int main() {
    int n;
    cin >> n;
    n = (1 << n);
    vector<int> a(n), b(n);
    cin >> a >> b;
    n *= 2;
    a.resize(n);
    b.resize(n);
    W1.resize(n + 1);
    W2.resize(n + 1);
    for (int i = 1; i <= n; i *= 2) {
        W1[i] = binpow(root, (mod - 1) / i, mod);
        W2[i] = binpow(W1[i], mod - 2, mod);
    }
    a = ntt(a);
    b = ntt(b);
    for (int i = 0; i < n; ++i) {
        a[i] = (a[i] * 1ll * b[i]) % mod;
    }
    a = ntt_inv(a);
    for (int i: a) {
        cout << i << " ";
    }
}