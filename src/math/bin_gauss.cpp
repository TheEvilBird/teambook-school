//n = number of equations, m = number of variables
int Gauss(int n, int m, vector<bitset<N>> a, bitset<N> &ans) {
    //reversing for lexocgraphically largest solution
    for (int i = 0; i < n; i++) {
        bitset<N> tmp;
        for (int j = 0; j < m; j++) tmp[j] = a[i][m - j - 1];
        tmp[m] = a[i][m];
        a[i] = tmp;
    }
    int rank = 0, det = 1;
    vector<int> pos(N, -1);
    for(int col = 0, row = 0; col < m && row < n; ++col) {
        int mx = row;
        for(int i = row; i < n; ++i) if(a[i][col]) { mx = i; break; }
        if(!a[mx][col]) { det = 0; continue; }
        swap(a[mx], a[row]);
        if (row != mx) { det = (det == 0 ? 0 : 1); }
        det &= a[row][col];
        pos[col] = row;
        //forward elimination
        for (int i = row + 1; i < n; ++i) {
            if (i != row && a[i][col]) { 
                a[i] ^= a[row]; 
            }
        }
        ++row, ++rank;
    }
    ans.reset();
    //backward substituition
    for (int i = m - 1; i >= 0; i--) {
        if (pos[i] == -1) {
            ans[i] = true;
        } else {
            int k = pos[i];
            for (int j = i + 1; j < m; j++) if (a[k][j]) ans[i] = ans[i] ^ ans[j];
            ans[i] = ans[i] ^ a[k][m];
        }
    }
    for (int i = rank; i < n; ++i) {
        if (a[i][m]) {
            return -1; //no solution
        }
    }
    //reversing again beacuse we reversed earlier
    bitset<N> tmp;
    for (int j = 0; j < m; j++) {
        tmp[j] = ans[m - j - 1];
    }
    ans = tmp;
    int free_var = 0;
    for(int i = 0; i < m; ++i) {
        if (pos[i] == -1) {
            free_var++;
        }
    }
    return free_var; //has solution
} 