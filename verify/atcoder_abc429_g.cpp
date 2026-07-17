#define PROBLEM "https://atcoder.jp/contests/abc429/tasks/abc429_g"
#include <bits/stdc++.h>
#include "math/modularsum.hpp"

using namespace std;
using namespace poe;

int main() {
    int tests;
    cin >> tests;
    while (tests--) {
        unsigned long long n, m, a, b, base;
        int modulus;
        cin >> n >> m >> a >> b >> base >> modulus;
        cout << sum_power_mod_linear(n, m, a, b, base, modulus) << '\n';
    }
}
