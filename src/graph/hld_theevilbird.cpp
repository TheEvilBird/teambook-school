struct HLD {

    // insert SegTree code
    struct SegTree {};

    int n, T;
    SegTree st;
    vector<vector<int>> tree;
    vector<int> par, siz, tin, tout, head;
    ll ans;

    HLD(int _n) {
        n = _n;
        tree.resize(n);
        par.resize(n, -1);
        siz.resize(n, 0);
        tin.resize(n);
        tout.resize(n);
        head.resize(n);
        st = SegTree(n);
    }

    void add_edge(int v, int u) {
        tree[v].emplace_back(u);
        tree[u].emplace_back(v);
    }

    void build(int v = 0) {
        dfs_siz(v, v);
        T = 0;
        head[v] = v;
        dfs_hld(v, v);
    }

    void dfs_siz(int v, int p) {
        par[v] = p;
        siz[v] = 1;
        for (auto &u : tree[v]) {
            if (u != p) {
                dfs_siz(u, v);
                siz[v] += siz[u];
            }
        }
        for (int i = 0; i < sz(tree[v]); ++i) {
            int x = tree[v][0], u = tree[v][i];
            if (x == p || siz[u] > siz[x]) {
                swap(tree[v][0], tree[v][i]);
            }
        }
    }

    void dfs_hld(int v, int p) {
        tin[v] = T++;
        for (auto u : tree[v]) {
            if (u == p) {
                continue;
            }
            if (u == tree[v][0]) {
                head[u] = head[v];
            } else {
                head[u] = u;
            }
            dfs_hld(u, v);
        }
        tout[v] = T;
    }

    void update(int v, int val) {
        st.update_segment(tin[v], tin[v], val);
    }

    bool is_anc(int v, int u) {
        return tin[v] <= tin[u] && tout[u] <= tout[v];
    }

    void go_up(int &v, int u) {
        while (!is_anc(head[v], u)) {
            ans = max(ans, st.get(tin[head[v]], tin[v]));
            v = par[head[v]];
        }
    }

    ll get(int v, int u) {// max on path
        ans = -INFLL;
        go_up(v, u);
        go_up(u, v);
        if (!is_anc(v, u)) {
            swap(v, u);
        }
        ans = max(ans, st.get(tin[v], tin[u]));
        return ans;
    }
};