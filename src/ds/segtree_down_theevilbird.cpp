struct SegTreeDown {

    struct Node {
        ll val = 0;

        Node() {}
        Node(ll val) : val(val) {}

        Node operator+(const Node &other) const {
            return Node(val + other.val);
        }

        void operator+=(const Node &other) {
            val += other.val;
        }
    };

    int n;
    vector<Node> tree;

    SegTreeDown(int _n) {
        n = _n;
        tree.assign(2 * n, Node());
    }

    void build(vector<ll> &a) {
        for (int i = 0; i < n; ++i) {
            tree[i + n] = Node(a[i]);
        }
        for (int i = n - 1; i >= 1; --i) {
            tree[i] = tree[2 * i] + tree[2 * i + 1];
        }
    }

    void update(int i, ll val) {
        i += n;
        tree[i] = val;
        i /= 2;
        while (i != 0) {
            tree[i] = tree[2 * i] + tree[2 * i + 1];
            i /= 2;
        }
    }

    ll get(int l, int r) {// [l, r)
        --r;
        l += n;
        r += n;
        Node ans;
        while (l <= r) {
            if (l % 2 == 1) {
                ans += tree[l];
                ++l;
            }
            if (r % 2 == 0) {
                ans += tree[r];
                --r;
            }
            l /= 2;
            r /= 2;
        }
        return ans.val;
    }
};