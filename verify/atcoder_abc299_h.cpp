#define PROBLEM "https://atcoder.jp/contests/abc299/tasks/abc299_h"
#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "math/randomwalk.hpp"

using namespace std;
using namespace poe;

int main() {
    long long remaining;
    cin >> remaining;
    const modint998244353 probability = modint998244353{1} / 6;
    cout << cyclic_hitting_expectation(
        1'000'000'000LL,
        remaining,
        vector<modint998244353>(6, probability)
    ) << '\n';
}
