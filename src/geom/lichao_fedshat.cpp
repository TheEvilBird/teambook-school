struct LiChao {// max
    struct Line {
        ll k = 0, b = -INFLL;

        Line() = default;

        Line(ll k, ll b) : k(k), b(b){};

        ll operator()(ll x) {
            return k * x + b;
        }
    };

    struct Node {
        Node *l = nullptr, *r = nullptr;
        Line cur;

        Node() = default;
    };

    Node *root = nullptr;
    int n = 1e9 + 1;

    void make_node(Node *&v) {
        if (v == nullptr) {
            v = new Node();
        }
    }

    void add(Node *&v, int l, int r, Line cur) {
        make_node(v);
        int m = (l + r) / 2;
        if (cur(m) > v->cur(m)) {
            swap(cur, v->cur);
        }
        if (l + 1 == r) {
            return;
        }
        if (cur(l) > v->cur(l)) {
            add(v->l, l, m, cur);
        } else {
            add(v->r, m, r, cur);
        }
    }

    void add(Line cur) {
        add(root, 0, n, cur);
    }

    ll get(Node *v, int l, int r, int x) {
        if (v == nullptr) {
            return -INFLL;
        }
        ll ans = v->cur(x);
        if (l + 1 == r) {
            return ans;
        }
        int m = (l + r) / 2;
        if (x < m) {
            ans = max(ans, get(v->l, l, m, x));
        } else {
            ans = max(ans, get(v->r, m, r, x));
        }
        return ans;
    }

    ll get(int x) {
        return get(root, 0, n, x);
    }
};
