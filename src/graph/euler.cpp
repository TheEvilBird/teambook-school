vector<vector<int>> g;
vector<bool> used;
vector<int> ed, tour;

void dfs(int v) {
    while (!g[v].empty()) {
        int u = g[v].back();
        g[v].pop_back();
        if (used[u]) {
            continue;
        }
        used[u] = 1;
        dfs(ed[u] ^ v);
    }
    tour.push_back(v);
}