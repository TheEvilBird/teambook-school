struct MaxFlow {
    struct Edge {
        ll flow, cap;
        int to, id;

        Edge() {}

        Edge(ll flow, ll cap, int to, int id) : flow(flow), cap(cap), to(to), id(id) {}
    };


    int n;
    vector<vector<Edge>> g;
    vector<int> d, head, used;
    ll max_cap;
    int s, t;

    MaxFlow() {}

    MaxFlow(int _n) {
        n = _n;
        g.resize(n);
    }

    void add_edge(int from, int to, ll cap) {
        g[from].emplace_back(0, cap, to, sz(g[to]));
        g[to].emplace_back(0, 0, from, sz(g[from]) - 1);
    }

    bool bfs() {
        d.assign(n, INF);
        d[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto e: g[v]) {
                if (d[e.to] == INF && e.cap - e.flow >= max_cap) {
                    d[e.to] = d[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return d[t] != INF;
    }

    ll dfs(int v, ll cur_flow) {
        if (v == t) {
            return cur_flow;
        }
        for (; head[v] < sz(g[v]); ++head[v]) {
            auto &e = g[v][head[v]];
            if (e.cap - e.flow >= max_cap && d[v] + 1 == d[e.to]) {
                ll new_flow = dfs(e.to, min(cur_flow, e.cap - e.flow));
                if (new_flow) {
                    e.flow += new_flow;
                    g[e.to][e.id].flow -= new_flow;
                    return new_flow;
                }
            }
        }
        return 0;
    }

    ll find_max_flow(int _s, int _t) {
        s = _s;
        t = _t;
        ll res = 0;
        for (int k = 30; k >= 0; --k) {
            max_cap = (1 << k);
            while (bfs()) {
                head.assign(n, 0);
                ll flow = 0;
                do {
                    flow = dfs(s, INFLL);
                    res += flow;
                } while (flow);
            }
        }
        return res;
    }

    ll dfs_const_flow(int v, ll cur_flow) {
        used[v] = 1;
        if (v == t) {
            return cur_flow;
        }
        for (auto &e: g[v]) {
            if (!used[e.to] && e.cap - e.flow > 0) {
                ll new_flow = dfs_const_flow(e.to, min(cur_flow, e.cap - e.flow));
                if (new_flow) {
                    e.flow += new_flow;
                    g[e.to][e.id].flow -= new_flow;
                    return new_flow;
                }
            }
        }
        return 0;
    }

    bool find_const_flow(int _s, int _t, ll F) {
        s = _s;
        t = _t;
        ll res = 0, flow = 0;
        max_cap = F;
        do {
            used.assign(n, 0);
            flow = dfs_const_flow(s, INF);
            res += flow;
        } while (flow && res < F);
        return res == F;
    }

    ll get_edge_flow(int v, int id) {
        return g[v][id].flow;
    }
};
