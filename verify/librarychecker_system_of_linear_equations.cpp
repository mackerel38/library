#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"
#include <bits/stdc++.h>
#include "math/gaussian.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    int equations, variables;
    cin >> equations >> variables;
    matrix<modint998244353> coefficients(equations, variables);
    for (int row = 0; row < equations; ++row) {
        for (auto& value : coefficients[row]) cin >> value;
    }
    vector<modint998244353> right(equations);
    for (auto& value : right) cin >> value;
    const auto result = solve_linear(coefficients, right);
    if (result.status == linearstatus::none) {
        cout << -1 << '\n';
        return 0;
    }
    cout << result.basis.size() << '\n';
    for (int i = 0; i < variables; ++i) cout << result.solution[i] << (i + 1 == variables ? '\n' : ' ');
    for (const auto& direction : result.basis) {
        for (int i = 0; i < variables; ++i) cout << direction[i] << (i + 1 == variables ? '\n' : ' ');
    }
}
