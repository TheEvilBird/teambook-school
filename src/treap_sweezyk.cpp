struct Node {
    int x, y, size, push, cnt;
    Node *l, *r;

    Node(int val) : x(val), y(rng()), size(1), push(0), cnt(0), l(nullptr), r(nullptr) {}
};

void push(Node *t) {
    if (t == nullptr) return;
    int p = t->push;
    if (p == 0) return;
    if (t->l != nullptr) {
        t->l->cnt += p;
        t->l->push += p;
    }
    if (t->r != nullptr) {
        t->r->cnt += p;
        t->r->push += p;
    }
    t->push = 0;
}

int size(Node *t) {
    return (t ? t->size : 0);
}

void update(Node *t) {
    if (t == nullptr) return;
    t->size = size(t->l) + size(t->r) + 1;
}

pair<Node *, Node *> split(Node *t, int k) {
    if (t == nullptr) return {nullptr, nullptr};
    if (k == 0) return {nullptr, t};
    push(t);
    if (size(t->l) + 1 <= k) {
        auto[l, r] = split(t->r, k - size(t->l) - 1);
        t->r = l;
        update(t);
        return {t, r};
    } else {
        auto[l, r] = split(t->l, k);
        t->l = r;
        update(t);
        return {l, t};
    }
}

Node *merge(Node *tl, Node *tr) {
    if (tl == nullptr) return tr;
    if (tr == nullptr) return tl;
    push(tl);
    push(tr);
    if (tl->y > tr->y) {
        tl->r = merge(tl->r, tr);
        update(tl);
        return tl;
    } else {
        tr->l = merge(tl, tr->l);
        update(tr);
        return tr;
    }
}

void dfs(Node *t) {
    if (t == nullptr) return;
    push(t);
    dfs(t->l);
    cout << t->x << ' ' << t->cnt << '\n';
    dfs(t->r);
}

void solve() {
    int n, m;
    cin >> n >> m;
    Node *root = nullptr;
    for (int i = 1; i <= n; i++) {
        Node *add = new Node(i);
        root = merge(root, add);
    }
    for (int i = 0; i < m; i++) {
        int l, r;
        cin >> l >> r;
        auto[L, R] = split(root, r);
        auto[L1, L2] = split(L, l - 1);
        L2->push = 1;
        L2->cnt += 1;
        root = merge(L2, merge(L1, R));
    }
    dfs(root);
}
