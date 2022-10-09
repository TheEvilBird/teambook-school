struct SegTree {

    static const ll off = 0;

    struct Node {
        ll val = 0;
        ll push = off;

        Node() {}

        Node(ll val) : val(val) {}

        Node operator+(const Node &other) const {
            return Node(val + other.val);
        }

        void operator+=(const Node &other) {
            val += other.val;
        }

        void use_push(int len = 1) {
            val += push * (ll) (len);
        }

        void update_push(ll pushed) {
            push += pushed;
        }
    };

    int n, qL, qR;
    ll val;
    Node ans;


    vector<Node> tree;
    vector<ll> a;

    SegTree() {}

    SegTree(int _n) {
        n = _n;
        tree.assign(4 * n, 0);
    }

    void update_vertex(int v, int l, int r) {// [l, r)
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        push(vL, l, m);
        push(vR, m, r);
        tree[v] = tree[vL] + tree[vR];
    }

    void push(int v, int l, int r) {// [l, r)
        if (tree[v].push == off) return;
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        tree[v].use_push(r - l);
        if (l + 1 != r) {
            tree[vL].update_push(tree[v].push);
            tree[vR].update_push(tree[v].push);
        }
        tree[v].push = off;
    }

    void build(vector<ll> &_a) {
        a = _a;
        build_tree(1, 0, n);
    }

    void build_tree(int v, int l, int r) {// [l, r)
        if (l + 1 == r) {
            tree[v] = Node(a[l]);
            return;
        }
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        build_tree(vL, l, m);
        build_tree(vR, m, r);
        update_vertex(v, l, r);
    }

    void update_segment(int _qL, int _qR, ll _val) {// [_qL, _qR]
        qL = _qL;
        qR = _qR + 1;
        val = _val;
        update_segment_tree(1, 0, n);
    }

    void update_segment_tree(int v, int l, int r) {// [l, r)
        push(v, l, r);
        if (qL <= l && r <= qR) {
            tree[v].update_push(val);
            push(v, l, r);
            return;
        }
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        if (qL < m) update_segment_tree(vL, l, m);
        if (m < qR) update_segment_tree(vR, m, r);
        update_vertex(v, l, r);
    }

    ll get(int _qL, int _qR) {// [_qL, _qR]
        qL = _qL;
        qR = _qR + 1;
        ans = Node();
        get_tree(1, 0, n);
        return ans.val;
    }

    void get_tree(int v, int l, int r) {// [l, r)
        push(v, l, r);
        if (qL <= l && r <= qR) {
            ans = ans + tree[v];
            return;
        }
        int m = (l + r) / 2, vL = 2 * v, vR = vL + 1;
        if (qL < m) get_tree(vL, l, m);
        if (m < qR) get_tree(vR, m, r);
    }
};