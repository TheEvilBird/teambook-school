void dfs(int v, int p = -1) {
    used[v] = true;
    tin[v] = fup[v] = timer++;
    int children = 0;
    for (auto to : g[v]) {
        if (to == p) {
            continue;
        }
        if (used[to]) {
            fup[v] = min(fup[v], tin[to]);
        } else {
            dfs(to, v);
            fup[v] = min(fup[v], fup[to]);
            if (fup[to] >= tin[v] && p != -1) {
                IS_CUTPOINT(v);
            }
            ++children;
        }
    }
    if (p == -1 && children > 1) {
        IS_CUTPOINT(v);
    }
}