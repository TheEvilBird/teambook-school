struct Node {
    int par;
    int par_c;
    int go[26];
    int term;
    int link;
    int super;
    int cnt;
};

const int N = 1e6 + 5;
int ptr = 1;
Node trie[N];

void add(string s) {
    int cur = 1;
    for (auto &q : s) {
        int c = q - 'a';
        if (trie[cur].go[c]) {
            cur = trie[cur].go[c];
        } else {
            ++ptr;
            trie[cur].go[c] = ptr;
            trie[ptr].par = cur;
            trie[ptr].par_c = c;
            cur = ptr;
        }
    }
    trie[cur].cnt++;
}

void build() {
    queue<int> q;
    q.push(1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        if (v != 1) {
            if (trie[v].par == 1) {
                trie[v].link = trie[v].super = 1;
            } else {
                trie[v].link = trie[trie[trie[v].par].link].go[trie[v].par_c];
                trie[v].super = (trie[trie[v].link].cnt ? trie[v].link : trie[trie[v].link].super);
            }
        }
        trie[v].cnt += trie[trie[v].link].cnt;
        for (int c = 0; c < 26; c++) {
            if (trie[v].go[c]) {
                q.push(trie[v].go[c]);
            } else {
                if (v == 1) {
                    trie[v].go[c] = 1;
                } else {
                    trie[v].go[c] = trie[trie[v].link].go[c];
                }
            }
        }
    }
}

trie[1].link = trie[1].super = trie[1].par = 1;
build();