struct Eq { // x = a (mod m)
    ll a, m;
 
    Eq() {};
 
    Eq(ll a, ll m) : a(a), m(m) {};
};
 
ll binpow(ll a, ll n, ll m) {
    if (n == 0) {
        return 1;
    }
    if (n % 2 == 0) {
        int128_t b = binpow(a, n / 2, m);
        return (b * b) % m;
    }
    int128_t x = binpow(a, n - 1, m);
    return (a * x) % m;
}
 
ll binpow(ll a, ll n) {
    if (n == 0) {
        return 1;
    }
    if (n % 2 == 0) {
        ll b = binpow(a, n / 2);
        return b * b;
    }
    return a * binpow(a, n - 1);
}
 
ll phi(ll a) {
    ll d = 2, k = a;
    map<ll, int> cnt;
    while (d * d <= a) {
        if (k % d == 0) {
            k /= d;
            ++cnt[d];
        } else {
            ++d;
        }
    }
    if (k != 1) {
        ++cnt[k];
    }
    ll ans = 1;
    for (auto i: cnt) {
        ans *= binpow(i.first, i.second - 1) * (i.first - 1);
    }
    return ans;
}
 
ll gcd(ll a, ll b) {
    return std::gcd(abs(a), abs(b));
}
 
Eq solve(Eq ai, Eq bi) {
    if (ai.m == -1 || bi.m == -1) {
        return {0, -1};
    }
    ll a = ai.m, b = bi.m, c = ai.a - bi.a;
    ll d = ::gcd(a, b);
    a /= d;
    b /= d;
    if (c % d != 0) {
        return {0, -1};
    }
    c /= d;
    ll x = (((int128_t) -c * (int128_t) binpow(a, phi(b) - 1, b)) % b + b) % b;
    x = ((int128_t) ai.m * (int128_t) x + ai.a) % lcm(ai.m, bi.m);
    return {x, lcm(ai.m, bi.m)};
}