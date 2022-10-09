struct Node {
    Node *ch[2] = {nullptr, nullptr};
    Node *par = nullptr;
    bool rev = false;
    int val, mn;
    int siz = 1;

    Node() {}

    Node(int val) : val(val), mn(val) {}
};

typedef Node *pnode;

int get_siz(pnode v) {
    return (v == nullptr ? 0 : v->siz);
}

int get_min(pnode v) {
    return (v == nullptr ? INF : v->mn);
}

void update(pnode v) {
    v->siz = 1 + get_siz(v->ch[0]) + get_siz(v->ch[1]);
    v->mn = min(v->val, min(get_min(v->ch[0]), get_min(v->ch[1])));
}

void push(pnode v) {
    if (v == nullptr || !v->rev) {
        return;
    }
    if (v->ch[0] != nullptr) {
        v->ch[0]->rev ^= 1;
    }
    if (v->ch[1] != nullptr) {
        v->ch[1]->rev ^= 1;
    }
    swap(v->ch[0], v->ch[1]);
    v->rev = 0;
}

bool is_root(pnode v) {
    return (v->par == nullptr ||
            (v->par->ch[0] != v && v->par->ch[1] != v));
}

int child_num(pnode v) {
    return (v->par->ch[1] == v);
}

void attach(pnode v, pnode p, int num) {
    if (v != nullptr) {
        v->par = p;
    }
    if (p != nullptr) {
        p->ch[num] = v;
    }
}

void rotate(pnode v) {
    int num = child_num(v);
    pnode p = v->par, vb = v->ch[num ^ 1];
    pnode g = (p == nullptr ? nullptr : p->par);
    if (g != nullptr) {
        if (!is_root(p)) {
            g->ch[child_num(p)] = v;
        }
    }
    v->par = g;
    attach(p, v, num ^ 1);
    attach(vb, p, num);
    update(p);
    update(v);
}

void splay(pnode v) {
    vector<pnode> st;
    pnode cur = v;
    st.emplace_back(cur);
    while (!is_root(cur)) {
        cur = cur->par;
        st.emplace_back(cur);
    }
    for (int i = sz(st) - 1; i >= 0; --i) {
        push(st[i]);
    }
    while (!is_root(v)) {
        if (!is_root(v->par)) {
            if (child_num(v) == child_num(v->par)) {
                rotate(v->par);
            } else {
                rotate(v);
            }
        }
        rotate(v);
    }
}

void expose(pnode v) {
    splay(v);
    v->ch[1] = nullptr;
    update(v);
    while (v->par != nullptr) {
        splay(v->par);
        attach(v, v->par, 1);
        update(v->par);
        splay(v);
    }
}

void make_root(pnode v) {
    expose(v);
    v->rev ^= 1;
}

void link(pnode v, pnode u) {
    make_root(v);
    make_root(u);
    u->par = v;
}

void cut(pnode v, pnode u) {
    make_root(v);
    make_root(u);
    push(u);
    u->ch[1] = nullptr;
    v->par = nullptr;
}

bool is_connected(pnode v, pnode u) {
    make_root(v);
    make_root(u);
    if (is_root(v) && u != v) {
        return false;
    } else {
        return true;
    }
}

int get_min(pnode v, pnode u) {
    make_root(v);
    make_root(u);
    return get_min(u);
}