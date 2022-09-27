struct Node {
  Node *l, *r;
  int mx;
  Node() {
    mx = -1;
    l = r = nullptr;
  }
};
 
const int N = 1e7 + 4e6;
const int LG = 20;
 
Node* nodes[N];
int ptr;
 
Node* new_node() {
  return nodes[ptr++];
}
 
Node* get_left(Node* t) {
  if (t && t->l) return t->l;
  return nullptr;
}
 
Node* get_right(Node* t) {
  if (t && t->r) return t->r;
  return nullptr;
}
 
int get_max(Node* t) {
  if (!t) return 0;
  return t->mx;
}
 
void update(int i, int val, Node* t, Node* old_t, int lx, int rx) {
  if (lx + 1 == rx) {
    t->mx = max(t->mx, val);
    return;
  }
  int m = (lx + rx) / 2;
  if (i < m) {
    t->l = new_Node();
    if (old_t && old_t->l) {
      t->l->mx = old_t->l->mx;
    }
    t->r = get_right(old_t);
    update(i, val, t->l, get_left(old_t), lx, m);
  } else {
    t->r = new_Node();
    if (old_t && old_t->r) {
      t->r->mx = old_t->r->mx;
    }
    t->l = get_left(old_t);
    update(i, val, t->r, get_right(old_t), m, rx);
  }
  t->mx = max(get_max(get_left(t)), get_max(get_right(t)));
};
 
int get(int l, int r, Node* t, int lx, int rx) {
  if (!t || lx >= r || rx <= l) return -1;
  if (lx >= l && rx <= r) return t->mx;
  int m = (lx + rx) / 2;
  return max(get(l, r, t->l, lx, m), get(l, r, t->r, m, rx));
}