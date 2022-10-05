int mod(int x, int m) {
    if (x < 0) x += m;
    if (x >= m) x -= m;
    return x;
}

vector<int> sufix_array(string s) {
    s += '$';
    int n = sz(s);
    vector<int> arr(n), narr(n), head(n), c(n), nc(n);
    for (int i = 0; i < n; ++i) {
        arr[i] = i;
    }
    sort(all(arr), [&](int x, int y) {
        return s[x] < s[y];
    });
    int cl = 0;
    c[arr[0]] = cl;
    head[0] = 0;
    for (int i = 1; i < n; ++i) {
        if (s[arr[i]] != s[arr[i - 1]]) {
            head[++cl] = i;
        }
        c[arr[i]] = cl;
    }
    for (int k = 1; k < n && cl < n; k *= 2) {
        for (int i = 0; i < n; i++) {
            int j = mod(arr[i] - k, n);
            narr[head[c[j]]++] = j;
        }
        cl = 0;
        head[0] = 0;
        pii prev = {c[narr[0]], c[mod(narr[0] + k, n)]};
        for (int i = 1; i < n; i++) {
            pii cur = {c[narr[i]], c[mod(narr[i] + k, n)]};
            if (cur != prev) {
                head[++cl] = i;
            }
            nc[narr[i]] = cl;
            prev = cur;
        }
        swap(arr, narr);
        swap(c, nc);
    }
    // returns sufix array without $
    return vector<int> (arr.begin() + 1, arr.end());
}

vector<int> build_lcp(string &s, vector<int> &suf) {
    int n = sz(s);
    vector<int> lcp(n - 1), order(n);
    for (int i = 0; i < n; ++i) {
        order[suf[i]] = i;
    }
    int l = 0;
    for (int i = 0; i < n; ++i) {
        int id = order[i];
        if (id + 1 == n) {
            l = 0;
            continue;
        }
        int j = suf[id + 1];
        if (l) --l;
        while (max(i + l, j + l) < n && 
               s[i + l] == s[j + l]) {
            ++l;
        }
        lcp[id] = l;
    }
    return lcp;
}