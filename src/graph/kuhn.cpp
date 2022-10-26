vector<vector<int>> g;
vector<int> used, mt;
int timer = 1;
bool dfs(int v) {
    if (used[v] == timer) {
        return false;
    }
    used[v] = timer;
    for (auto u : g[v]) {
        if (mt[u] == -1) {
            mt[u] = v;
            return true;
        }
    }
    for (auto u : g[v]) {
        if (dfs(mt[u])) {
            mt[u] = v;
            return true;
        }
    }
    return false;
}
for (int i = 0; i < n; ++i) {
    if (dfs(i)) {
        ++timer;
    }
}