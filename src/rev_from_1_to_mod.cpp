r[1] = 1;
for (int i = 2; i < m; ++i) {
    r[i] = (m - (m / i) * r[m % i] % m) % m;
}