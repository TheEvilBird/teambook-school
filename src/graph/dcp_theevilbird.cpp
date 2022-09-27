struct Query {
    char type;
    int v, u;

    Query(char type) : type(type) {}
    Query(char type, int v, int u) : type(type), v(v), u(u) {}
};


struct DCP {
    int n, k, ans; // n - vertex, k - queries
    vector<int> par, rk;
    vector<pair<pii, int>> hist;
    // 0 - par, 1 - rk, 2 - ans;

    int qL, qR;
    pii edge;
    vector<vector<pii>> tree;
    vector<Query> qs;

    DCP(int _n, int _k) {
        n = ans = _n;
        par.resize(n);
        rk.resize(n, 1);
        for (int i = 0; i < n; ++i) par[i] = i;
        k = _k;
        tree.assign(4 * k, vector<pii>());
    }

    int dsu_get(int v) {
        while (par[v] != v) v = par[v];
        return v;
    }

    void dsu_unite(int a, int b) {
        a = dsu_get(a);
        b = dsu_get(b);
        if (a == b) return;
        if (rk[a] > rk[b]) swap(a, b);
        hist.emplace_back((pii){0, a}, par[a]);
        hist.emplace_back((pii){2, -1}, ans);
        par[a] = b;
        --ans;
        if (rk[a] == rk[b]) {
            hist.emplace_back((pii){1, b}, rk[b]);
            ++rk[b];
        }
    }

    void dsu_unite(pii e) {
        dsu_unite(e.first, e.second);
    }

    void cancel(pair<pii, int> &el) {
        int &type = el.first.first;
        int &id = el.first.second;
        int &val = el.second;
        if (type == 0) {
            par[id] = val;
        }
        else if (type == 1) {
            rk[id] = val;
        }
        else if (type == 2) {
            ans = val;
        }
    }

    void add_edge(int _qL, int _qR, pii e) { // [L, R]
        qL = _qL;
        qR = _qR + 1;
        edge = e;
        add_edge_tree(1, 0, k);
    }

    void add_edge_tree(int v, int l, int r) {
        if (qL <= l && r <= qR) {
            tree[v].emplace_back(edge);
            return;
        }
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        if (qL < m) add_edge_tree(vL, l, m);
        if (m < qR) add_edge_tree(vR, m, r);
    }

    void go(vector<Query> &_qs) {
        qs = _qs;
        go_tree(1, 0, k);
    }

    void go_tree(int v, int l, int r) {
        int siz = sz(hist);
        for (auto &e: tree[v]) {
            dsu_unite(e);
        }
        if (l + 1 == r) {
            if (qs[l].type == '?') {
                cout << ans << en;
            }
        }
        else {
            int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
            go_tree(vL, l, m);
            go_tree(vR, m, r);
        }
        while (sz(hist) > siz) {
            cancel(hist.back());
            hist.pop_back();
        }
    }
};

void solve() {
    int n, k;
    cin >> n >> k;
    DCP dcp(n, k);
    set<pair<pii, int>> edges;
    vector<Query> qs;
    for (int i = 0; i < k; ++i) {
        char tp;
        cin >> tp;
        if (tp == '?') {
            qs.emplace_back(tp);
        }
        else {
            int v, u;
            cin >> v >> u;
            --v; --u;
            if (v > u) swap(v, u);
            qs.emplace_back(tp, v, u);
            if (tp == '+') {
                edges.emplace((pii){v, u}, i);
            }
            else {
                auto it = edges.lower_bound({(pii){v, u}, 0});
                dcp.add_edge(it->second, i, it->first);
                edges.erase(it);
            }
        }
    }
    for (auto &e: edges) {
        dcp.add_edge(e.second, k - 1, e.first);
    }
    if (k) dcp.go(qs);

}