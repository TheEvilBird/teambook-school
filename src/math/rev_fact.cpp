int inv(int a, int m) {
    if (a == 1)
        return 1;
    return (1 - inv(m % a, a) * m) / a + m;
}

{
    f[0] = 1;
    for (int i = 1; i < N; i++) {
        f[i] = i * f[i - 1] % mod;
    }

    r[N - 1] = inv(f[N - 1]) for (int i = N - 1; i >= 1; i--) {
        r[i - 1] = r[i] * i % mod;
    }
}