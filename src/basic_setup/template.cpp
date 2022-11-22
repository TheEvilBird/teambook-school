#include <bits/stdc++.h>

using namespace std;

#define sz(x) (int) ((x).size())
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

typedef long long ll;
typedef __int128 int128;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const char en = '\n';
const int INF = 1e9 + 7;
const ll INFLL = 1e18;

mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());

template<class T>
istream &operator>>(istream &is, vector<T> &a) {
    for (auto &i : a) {
        is >> i;
    }
    return is;
}

#ifdef LOCAL
#include "debug.h"
#else
#define debug(...) 42
#endif

void solve() {
}

int32_t main() {
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
#else
    ios_base::sync_with_stdio(0);
    cin.tie(0);
#endif
    solve();
    return 0;
}
