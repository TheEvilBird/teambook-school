ll binpow(ll a, ll n, ll mod) {
    if (n == 0) {
        return 1;
    }
    if (n % 2 == 0) {
        int128_t b = binpow(a, n / 2, mod);
        return (b * b) % mod;
    }
    return (((int128_t) a) * binpow(a, n - 1, mod)) % mod;
}

constexpr int N = 1e7;
vector<int> pr, lp;

bool prime(ll n) {
    if (n <= N) {
        return binary_search(all(pr), n);
    }
    int iter = 60;
    int s = 0;
    ll d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
        ++s;
    }
    auto test = [&](ll a) {
        if (binpow(a, d, n) == 1) {
            return true;
        }
        ll _2r = 1;
        for (int r = 0; r < s; ++r) {
            auto tmp = binpow(binpow(a, d, n), _2r, n);
            if (tmp == n - 1) {
                return true;
            }
            _2r *= 2;
        }
        return false;
    };
    for (int _ = 0; _ < iter; ++_) {
        ll a = uniform_int_distribution<ll>(1, n - 1)(rnd);
        if (!test(a)) {
            return false;
        }
    }
    return true;
}

ll f(ll x, ll n) {
    return ((int128_t) x * (int128_t) x + (int128_t) 2) % n;
}

ll pollard(ll n) {
    ll a = uniform_int_distribution<ll>(0, n - 1)(rnd);
    ll x = a, y = a, d = 1;
    constexpr int iter = 5e4;
    for (int _ = 0; _ < iter; ++_) {
        x = f(f(x, n), n);
        y = f(y, n);
        d = gcd(abs(x - y), n);
        if (d != 1 && d != n) {
            break;
        }
    }
    if (d == 1 || d == n) {
        pollard(n);
    }
    return d;
}

vector<ll> res;

void factor(ll n) {
    if (n <= N) {
        while (true) {
            if (lp[n] == 0) {
                break;
            }
            res.push_back(lp[n]);
            n /= lp[n];
        }
        return;
    }
    if (prime(n)) {
        res.push_back(n);
        return;
    }
    ll d = pollard(n);
    factor(n / d);
    factor(d);
}

void solve() {
    ll n;
    cin >> n;
    lp.resize(N + 1);
    for (int i = 2; i <= N; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            pr.push_back(i);
        }
        for (int j = 0; j < (int) pr.size() && pr[j] <= lp[i] && i * pr[j] <= N; ++j) {
            lp[i * pr[j]] = pr[j];
        }
    }
    factor(n);
}

