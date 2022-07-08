struct MinCostMaxFlow {
    struct Edge {
        ll flow, cap, price;
        int to, id;

        Edge() {}

        Edge(ll flow, ll cap, ll price, int to, int id) : flow(flow), cap(cap), price(price), to(to), id(id) {}

    };


    int n;
    int s, t;
    ll ans;
    vector<vector<Edge>> g;
    vector<int> d;
    vector<ll> add_f;
    vector<pii> par;

    MinCostMaxFlow() {}

    MinCostMaxFlow(int _n) {
        n = _n;
        g.resize(n);
    }

    void add_edge(int from, int to, ll cap, ll price) {
        g[from].emplace_back(0, cap, price, to, sz(g[to]));
        g[to].emplace_back(0, 0, -price, from, sz(g[from]) - 1);
    }
    
    ll get_edge_flow(int v, int id) {
        return g[v][id].flow;
    }

    void FB() {
        d.assign(n, INF);
        add_f.assign(n, 0);
        par.assign(n, {-1, -1});
        d[s] = 0;
        add_f[0] = INF;
        queue<int> q;
        q.push(s);
        vector<int> used(n, 0);
        used[s] = 1;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            used[v] = 0;
            for (int i = 0; i < sz(g[v]); ++i) {
                auto &e = g[v][i];
                if (e.flow < e.cap && d[e.to] > d[v] + e.price) {
                    d[e.to] = d[v] + e.price;
                    add_f[e.to] = min(add_f[v], e.cap - e.flow);
                    par[e.to] = {v, i};
                    if (!used[e.to]) {
                        q.push(e.to);
                        used[e.to] = 1;
                    }
                }
            }
        }
    }

    void push_flow(ll flow) {
        int cur = t;
        while (cur != s) {
            int prev = par[cur].first, id = par[cur].second;
            g[prev][id].flow += flow;
            g[cur][g[prev][id].id].flow -= flow;
            ans += g[prev][id].price * flow;
            cur = prev;
        }
    }

    ll min_cost_max_flow(int _s, int _t) {
        ans = 0;
        s = _s; t = _t;
        while (true) {
            FB();
            ll flow = add_f[t];
            if (flow == 0) {
                break;
            }
            push_flow(flow);
        }
        return ans;
    }
};