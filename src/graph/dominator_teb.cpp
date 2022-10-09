struct Edge {
    int from, to, id;

    Edge() = default;
    Edge(int from, int to, int id) : from(from), to(to), id(id) {}
};

struct DSU {
    int n;
    vector<int> par;
    vector<pii> mn;

    DSU() = default;
    DSU(int n) : n(n) {
        par.resize(n);
        mn.resize(n);
        init();
    }

    void init() {
        for (int i = 0; i < n; ++i) {
            par[i] = i;
            mn[i] = {INF, i};
        }
    }

    int get(int v) {
        if (par[v] == v) {
            return v;
        }
        int p = get(par[v]);
//        mn[v] = min(mn[v], mn[par[v]]);
        if (mn[par[v]].first < mn[v].first) {
            mn[v] = mn[par[v]];
        }
        par[v] = p;
        return p;
    }

    void unite(int a, int b) {
        par[a] = b;
    }
};

struct DominatorTree {
    int n;
    vector<Edge> edges;
    vector<int> sdom, idom, tin, order, par, used, dp;
    vector<vector<int>> g, rg, queries;
    DSU dsu_sdom, dsu_idom;

    DominatorTree() = default;
    DominatorTree(int n) : n(n), dsu_sdom(n), dsu_idom(n) {
        sdom.resize(n, INF); // semi-dominator
        idom.resize(n, INF); // immediate dominator
        tin.resize(n, -1);
        par.resize(n);
        used.resize(n, 0);
        dp.resize(n, INF);
        g.resize(n);
        rg.resize(n);
        queries.resize(n);
    }

    void add_edge(int from, int to) {
        edges.emplace_back(from, to, sz(edges));
    }

    void dfs(int v) {
        tin[v] = sz(order);
        order.emplace_back(v);
        for (auto i: g[v]) {
            const auto &e = edges[i];
            if (tin[e.to] == -1) {
                par[e.to] = v;
                dfs(e.to);
            }
        }
    }

    void dfs_idom(int v) {
        used[v] = 1;
        for (auto i: g[v]) {
            const auto &e = edges[i];
            if (!used[e.to]) {
                dfs_idom(e.to);
            }
        }
        for (auto u: queries[v]) {
            dsu_idom.get(u);
            dp[u] = dsu_idom.mn[u].second;
        }
        dsu_idom.mn[v] = {sdom[v], v};
        for (auto i: g[v]) {
            const auto &e = edges[i];
            if (par[e.to] == v) {
                dsu_idom.unite(e.to, v);
            }
        }
    }

    void build(int s) {
        for (int i = 0; i < sz(edges); ++i) {
            g[edges[i].from].emplace_back(i);
            rg[edges[i].to].emplace_back(i);
        }
        // reorder vertex
        dfs(s);

        // build sdom
        for (int _ = sz(order) - 1; _ >= 0; --_) {
            int v = order[_];
            if (v == s) {
                continue;
            }
            for (auto i: rg[v]) {
                const auto &e = edges[i];
                if (tin[e.from] == -1) {
                    continue;
                }
                if (tin[e.from] < tin[v]) {
                    sdom[v] = min(sdom[v], tin[e.from]);
                }
                else {
                    int u = dsu_sdom.get(e.from);
                    sdom[v] = min(sdom[v], dsu_sdom.mn[e.from].first);
                }
            }
            dsu_sdom.mn[v] = {sdom[v], v};
            for (auto i: g[v]) {
                const auto &e = edges[i];
                if (v == par[e.to]) {
                    dsu_sdom.unite(e.to, v);
                }
            }
        }

        // build queries for idoms
        for (int i = 0; i < n; ++i) {
            if (i == s || sdom[i] == INF || tin[i] == -1) {
                continue;
            }
            queries[order[sdom[i]]].emplace_back(i);
        }
        dfs_idom(s);

        // build idom
        idom[s] = tin[s];
        for (auto v: order) {
            if (v == s) {
                continue;
            }
            if (v == dp[v]) {
                idom[v] = sdom[v];
            }
            else {
                idom[v] = idom[dp[v]];
            }
        }
    }

    int get_idom(int v) {
        return (idom[v] == INF ? -1 : order[idom[v]]);
    }
};