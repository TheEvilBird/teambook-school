struct Line {
    ll k = 0, b = -INFLL;
    Line() = default;
    Line(ll k, ll b) : k(k), b(b){};
 
    ld operator()(ld x) {
        return k * x + b;
    }
};
 
bool operator<(Line a, Line b) {
    return a.k < b.k || (a.k == b.k && a.b > b.b);
}
 
bool operator<(pair<Line, ld> a, pair<Line, ld> b) {
    return a.second < b.second;
}

struct CHT {
    vector<pair<Line, ld>> convex;
 
    void add(Line a) {
        while (!convex.empty() && a(convex.back().second) > convex.back().first(convex.back().second)) {
            convex.pop_back();
        }
        ld xn = -INFLL;
        if (!convex.empty()) {
            xn = (a.b - convex.back().first.b + 0.0) / (convex.back().first.k - a.k);
        }
        convex.push_back({a, xn});
    }
 
    CHT(vector<Line> lines) {
        sort(all(lines));
        for (int i = 0; i < (int) lines.size();) {
            int j = i;
            while (j < (int) lines.size() && lines[i].k == lines[j].k) {
                ++j;
            }
            add(lines[i]);
            i = j;
        }
    }
 
    ld get(ld x) {
        auto it = upper_bound(all(convex), pair(Line(), x)) - convex.begin();
        return convex[it - 1].first(x);
    }
};
 