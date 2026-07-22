#define PROBLEM "https://atcoder.jp/contests/abc283/tasks/abc283_h"
#include <bits/stdc++.h>
#include "math/bitcountsum.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        long long limit, modulus, remainder;
        cin >> limit >> modulus >> remainder;
        const long long first = remainder == 0 ? modulus : remainder;
        const long long count = (limit - first) / modulus + 1;
        cout << sum_popcount_arithmetic_progression(count, first, modulus) << '\n';
    }
}
