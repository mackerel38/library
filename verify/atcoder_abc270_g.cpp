#define PROBLEM "https://atcoder.jp/contests/abc270/tasks/abc270_g"
#include <bits/stdc++.h>
#include "math/discretelog.hpp"

using namespace std;
using namespace poe;

int main() {
    int tests;
    cin >> tests;
    while (tests--) {
        long long mod, multiplier, addition, start, goal;
        cin >> mod >> multiplier >> addition >> start >> goal;
        if (start == goal) {
            cout << 0 << '\n';
        } else if (multiplier == 0) {
            cout << (addition == goal ? 1 : -1) << '\n';
        } else if (multiplier == 1) {
            if (addition == 0) cout << -1 << '\n';
            else cout << detail::safe_mod(goal - start, mod) * inv_mod(addition, mod) % mod << '\n';
        } else {
            const long long fixed = addition * inv_mod(1 - multiplier, mod) % mod;
            const long long initial = detail::safe_mod(start - fixed, mod);
            const long long target = detail::safe_mod(goal - fixed, mod);
            if (initial == 0) cout << -1 << '\n';
            else cout << discrete_log(multiplier, target * inv_mod(initial, mod) % mod, mod) << '\n';
        }
    }
}
