struct SegTree {
    struct Node {
        ll max, sec_max;
        int cnt_max;
        ll min, sec_min;
        int cnt_min;
        ll sum;
        ll push_add;
        ll push_eq;
    };

    int n, qL, qR;
    ll val, ans;

    vector<Node> tree;
    vector<ll> a;

    SegTree(int _n) {
        n = _n;
        tree.assign(4 * n, {0, -INFLL, 1, 0, INFLL, 1, 0, 0, -1});
    }

    void update_vertex(int v, int l, int r) {
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;

        tree[v].sum = tree[vL].sum + tree[vR].sum;

        tree[v].max = max(tree[vL].max, tree[vR].max);
        tree[v].sec_max = max(tree[vL].sec_max, tree[vR].sec_max);
        tree[v].cnt_max = 0;
        if (tree[vL].max == tree[v].max) {
            tree[v].cnt_max += tree[vL].cnt_max;
        }
        else {
            tree[v].sec_max = max(tree[v].sec_max, tree[vL].max);
        }
        if (tree[vR].max == tree[v].max) {
            tree[v].cnt_max += tree[vR].cnt_max;
        }
        else {
            tree[v].sec_max = max(tree[v].sec_max, tree[vR].max);
        }

        tree[v].min = min(tree[vL].min, tree[vR].min);
        tree[v].sec_min = min(tree[vL].sec_min, tree[vR].sec_min);
        tree[v].cnt_min = 0;
        if (tree[vL].min == tree[v].min) {
            tree[v].cnt_min += tree[vL].cnt_min;
        }
        else {
            tree[v].sec_min = min(tree[v].sec_min, tree[vL].min);
        }
        if (tree[vR].min == tree[v].min) {
            tree[v].cnt_min += tree[vR].cnt_min;
        }
        else {
            tree[v].sec_min = min(tree[v].sec_min, tree[vR].min);
        }
    }

    void recalc_eq(int v, int l, int r, ll cur) {
        tree[v].max = tree[v].min = tree[v].push_eq = cur;
        tree[v].sec_max = -INFLL;
        tree[v].sec_min = INFLL;
        tree[v].cnt_max = tree[v].cnt_min = r - l;
        tree[v].sum = cur * (ll)(r - l);
        tree[v].push_add = 0;
    }

    void recalc_add(int v, int l, int r, ll cur) {
        if (tree[v].min == tree[v].max) {
            recalc_eq(v, l, r, tree[v].max + cur);
            return;
        }
        tree[v].max += cur;
        if (tree[v].sec_max != -INFLL) {
            tree[v].sec_max += cur;
        }

        tree[v].min += cur;
        if (tree[v].sec_min != INFLL) {
            tree[v].sec_min += cur;
        }

        tree[v].sum += (ll)(r - l) * cur;
        tree[v].push_add += cur;
    }

    void recalc_min(int v, int l, int r, ll cur) {
        if (tree[v].min >= cur) {
            recalc_eq(v, l, r, cur);
            return;
        }
        if (tree[v].max <= cur) return;
        if (tree[v].sec_min == tree[v].max) {
            tree[v].sec_min = cur;
        }
        tree[v].sum -= (ll)(tree[v].max - cur) * 
                       (ll)(tree[v].cnt_max);
        tree[v].max = cur;
    }

    void recalc_max(int v, int l, int r, ll cur) {
        if (tree[v].max <= cur) {
            recalc_eq(v, l, r, cur);
            return;
        }
        if (tree[v].min >= cur) return;
        if (tree[v].sec_max == tree[v].min) {
            tree[v].sec_max = cur;
        }
        tree[v].sum += (ll)(tree[v].max - cur) * 
                       (ll)(tree[v].cnt_max);
        tree[v].min = cur;
    }

    void push(int v, int l, int r) {
        if (l + 1 == r) return;
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        if (tree[v].push_eq != -1) {
            recalc_eq(vL, l, m, tree[v].push_eq);
            recalc_eq(vR, m, r, tree[v].push_eq);
            tree[v].push_eq = -1;
            return;
        }

        recalc_add(vL, l, m, tree[v].push_add);
        recalc_add(vR, m, r, tree[v].push_add);
        tree[v].push_add = 0;

        recalc_min(vL, l, m, tree[v].max);
        recalc_min(vR, m, r, tree[v].max);

        recalc_max(vL, l, m, tree[v].min);
        recalc_max(vR, m, r, tree[v].min);
    }

    void build(vector<ll> &_a) {
        a = _a;
        build_tree(1, 0, n);
    }

    void build_tree(int v, int l, int r) {
        if (l + 1 == r) {
            tree[v] = {a[l], -INFLL, 1, a[l], 
                       INFLL, 1, a[l], 0, -1};
            return;
        }
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        build_tree(vL, l, m);
        build_tree(vR, m, r);
        update_vertex(v, l, r);
    }

    void update_segment_min(int _qL, int _qR, ll _val) {
        qL = _qL;
        qR = _qR + 1;
        val = _val;
        update_segment_min_tree(1, 0, n);
    }

    void update_segment_min_tree(int v, int l, int r) {
        if (tree[v].max <= val) return;
        if (qL <= l && r <= qR && tree[v].sec_max < val) {
            recalc_min(v, l, r, val);
            return;
        }
        push(v, l, r);
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        if (qL < m) update_segment_min_tree(vL, l, m);
        if (m < qR) update_segment_min_tree(vR, m, r);
        update_vertex(v, l, r);
    }

    void update_segment_max(int _qL, int _qR, ll _val) {
        qL = _qL;
        qR = _qR + 1;
        val = _val;
        update_segment_max_tree(1, 0, n);
    }

    void update_segment_max_tree(int v, int l, int r) {
        if (tree[v].min >= val) return;
        if (qL <= l && r <= qR && tree[v].sec_min > val) {
            recalc_max(v, l, r, val);
            return;
        }
        push(v, l, r);
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        if (qL < m) update_segment_max_tree(vL, l, m);
        if (m < qR) update_segment_max_tree(vR, m, r);
        update_vertex(v, l, r);
    }

    void update_segment_add(int _qL, int _qR, ll _val) {
        qL = _qL;
        qR = _qR + 1;
        val = _val;
        update_segment_add_tree(1, 0, n);
    }

    void update_segment_add_tree(int v, int l, int r) {
        if (qL <= l && r <= qR) {
            recalc_add(v, l, r, val);
            return;
        }
        push(v, l, r);
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        if (qL < m) update_segment_add_tree(vL, l, m);
        if (m < qR) update_segment_add_tree(vR, m, r);
        update_vertex(v, l, r);
    }

    ll get(int _qL, int _qR) {
        qL = _qL;
        qR = _qR + 1;
        ans = 0;
        get_tree(1, 0, n);
        return ans;
    }

    void get_tree(int v, int l, int r) {
        if (qL <= l && r <= qR) {
            ans += tree[v].sum;
            return;
        }
        push(v, l, r);
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        if (qL < m) get_tree(vL, l, m);
        if (m < qR) get_tree(vR, m, r);
    }

    void print_all() {
        print_all_tree(1, 0, n);
    }

    void print_all_tree(int v, int l, int r) {
        if (l + 1 == r) {
            cout << tree[v].sum << en;
            return;
        }
        push(v, l, r);
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        print_all_tree(vL, l, m);
        print_all_tree(vR, m, r);
    }
};
