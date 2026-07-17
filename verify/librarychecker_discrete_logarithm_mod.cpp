#define PROBLEM "https://judge.yosupo.jp/problem/discrete_logarithm_mod"
#include <bits/stdc++.h>
#include "math/discretelog.hpp"

using namespace std;
using namespace poe;

int main() {
    int queries;
    cin >> queries;
    while (queries--) {
        long long base, target, mod;
        cin >> base >> target >> mod;
        cout << discrete_log(base, target, mod) << '\n';
    }
}
