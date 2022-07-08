struct Node {
    int go[26];
    int suf, prev, term, len;

    Node() {
        for (auto &i: go) {
            i = -1;
        }
        len = 0;
        suf = -1;
        prev = -1;
        term = 0;
    }
};

vector<Node> automat;

int add(int a, int ch) {
    int b = sz(automat);
    automat.emplace_back();
    automat[b].prev = a;
    automat[b].suf = 0;
    automat[b].len = automat[a].len + 1;
    for (; a != -1; a = automat[a].suf) {
        if (automat[a].go[ch] == -1) {
            automat[a].go[ch] = b;
            continue;
        }
        int c = automat[a].go[ch];
        if (automat[c].prev == a) {
            automat[b].suf = c;
            break;
        }
        int d = sz(automat);
        automat.emplace_back();
        automat[d].suf = automat[c].suf;
        automat[d].len = automat[a].len + 1;
        automat[c].suf = d;
        automat[b].suf = d;
        automat[d].prev = a;
        for (int i = 0; i < 26; ++i) {
            automat[d].go[i] = automat[c].go[i];
        }
        for (; a != -1 && automat[a].go[ch] == c; a = automat[a].suf) {
            automat[a].go[ch] = d;
        }
        break;
    }
    // returns id of the added vertex
    return b;
}