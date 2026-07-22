#define PROBLEM "https://atcoder.jp/contests/abc277/tasks/abc277_h"
#include <bits/stdc++.h>
#include "graph/integerconstraints.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, maximum_value, q;
    cin >> n >> maximum_value >> q;
    boundedsumsat sat(n, maximum_value);
    while (q--) {
        int left, right, lower, upper;
        cin >> left >> right >> lower >> upper;
        sat.add_sum_constraint(left - 1, right - 1, lower, upper);
    }
    if (!sat.satisfiable()) {
        cout << -1 << '\n';
        return 0;
    }
    for (int index = 0; index < n; ++index) {
        cout << sat[index] << (index + 1 == n ? '\n' : ' ');
    }
}
