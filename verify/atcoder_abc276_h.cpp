#define PROBLEM "https://atcoder.jp/contests/abc276/tasks/abc276_h"
#include <bits/stdc++.h>
#include "math/rectangleparity.hpp"
#include "structure/prefixsum.hpp"

using namespace std;
using namespace poe;

struct condition {
    int top, left, bottom, right, value;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<condition> conditions(q);
    rectangleparitysystem parity(n, n);
    differencearray2d<int> coverage(n, n);
    for (auto& [top, left, bottom, right, value] : conditions) {
        cin >> top >> bottom >> left >> right >> value;
        --top;
        --left;
        if (value != 0) {
            parity.add_constraint(top, left, bottom, right, value - 1);
            coverage.add(top, left, bottom, right, 1);
        }
    }
    if (!parity.satisfiable()) {
        cout << "No\n";
        return 0;
    }
    coverage.build();
    vector<vector<int>> covered(n, vector<int>(n));
    vector<vector<int>> answer(n, vector<int>(n));
    for (int row = 0; row < n; ++row) {
        for (int column = 0; column < n; ++column) {
            covered[row][column] = coverage(row, column) > 0;
            if (covered[row][column]) answer[row][column] = parity(row, column) + 1;
        }
    }
    prefixsum2d<int> covered_sum(covered);
    for (const auto& [top, left, bottom, right, value] : conditions) {
        if (value == 0 && covered_sum.sum(top, left, bottom, right) ==
                              (bottom - top) * (right - left)) {
            cout << "No\n";
            return 0;
        }
    }
    cout << "Yes\n";
    for (const auto& row : answer) {
        for (int column = 0; column < n; ++column) {
            cout << row[column] << (column + 1 == n ? '\n' : ' ');
        }
    }
}
