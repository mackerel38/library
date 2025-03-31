#pragma once
#include <bits/stdc++.h>
using namespace std;
// 素数判定 O(log n)
bool isprime(long long n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    int flag = 1;
    if (n < 4759123141LL) flag = 0;
    vector<vector<long long>> tests = {{2, 7, 61}, {2, 325, 9375, 28178, 450775, 9780504, 1795265022}};
    long long s = 0, d = n - 1;
    while (d % 2 == 0) {
        ++s;
        d >>= 1;
    }
    for (auto a : tests[flag]) {
        if (n <= a) return true;
        __int128_t x = 1, a2=a%n, d2 = d;
        while (d2) {
            if (d2 & 1) x = x * a2 % n;
            a2 = a2 * a2 % n;
            d2 >>= 1;
        }
        if (x != 1) {
            long long t;
            for (t = 0; t < s; ++t) {
                if (x == n - 1) break;
                x = x * x % n;
            }
            if (t == s) return false;
        }
    }
    return true;
}
vector<int> primes;
// n 以下の素数をprimes に列挙する O(n log log n)
int enumprimes(int n) {
    vector<bool> primeflag(n+1);
    for (long long i=2; i<=n; i++) {
        if (primeflag[i]) continue;
        primes.push_back(i);
        for (long long j=i*i; j<=n; j+=i) {
            primeflag[j] = true;
        }
    }
    return primes.size();
}