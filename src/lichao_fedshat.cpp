struct Lichao { // min
    struct Line {
        ll k, b;

        Line() : k(0), b(INFLL) {};

        Line(ll k, ll b) : k(k), b(b) {};

        ll operator()(ll x) {
            return k * x + b;
        }

        bool operator==(Line a) {
            return a.k == k && a.b == b;
        }

        bool operator!=(Line a) {
            return !(a == *this);
        }
    };

    struct Node {
        Node *l = nullptr, *r = nullptr;
        Line cur;

        Node() = default;
    };

    Node *root = nullptr;
    int n = 1e5 + 1;

    void createnode(Node *&v) {
        if (v == nullptr) {
            v = new Node();
        }
    }

    void add(Node *&v, int l, int r, Line cur) {
        createnode(v);
        if (l + 1 == r) {
            if (cur(l) < v->cur(l))
                v->cur = cur;
            return;
        }
        int m = (l + r) / 2;
        if (v->cur.k == cur.k) {
            v->cur = {cur.k, min(v->cur.b, cur.b)};
            return;
        }
        ll xc = (cur.b - v->cur.b) / (v->cur.k - cur.k);
        auto prev = v->cur;
        if (xc < m) {
            if (v->cur.k < cur.k)
                add(v->l, l, m, cur);
            else {
                v->cur = cur;
                add(v->l, l, m, prev);
            }
        } else {
            if (v->cur.k < cur.k) {
                v->cur = cur;
                add(v->r, m, r, prev);
            } else
                add(v->r, m, r, cur);
        }
    }

    void add(Line cur) {
        add(root, -n, n, cur);
    }

    ll get(Node *v, int l, int r, int x) {
        if (l + 1 == r) {
            return v->cur(x);
        }
        int m = (l + r) / 2;
        ll ans = v->cur(x);
        if (x < m && v->l != nullptr) {
            ans = min(ans, get(v->l, l, m, x));
        } else if (x >= m && v->r != nullptr) {
            ans = min(ans, get(v->r, m, r, x));
        }
        return ans;
    }

    ll get(int x) {
        return get(root, -n, n, x);
    }
};