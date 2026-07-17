#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations_mod_2"
#include <bits/stdc++.h>
#include "math/gaussian.hpp"

using namespace std;
using namespace poe;

void print_bits(const vector<int>& bits) {
    for (int bit : bits) cout << bit;
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int equations, variables;
    cin >> equations >> variables;
    matrix<int> coefficients(equations, variables);
    for (int row = 0; row < equations; ++row) {
        string bits;
        cin >> bits;
        for (int column = 0; column < variables; ++column) {
            coefficients[row][column] = bits[column] - '0';
        }
    }
    string bits;
    cin >> bits;
    vector<int> right(equations);
    for (int row = 0; row < equations; ++row) right[row] = bits[row] - '0';

    const auto result = solve_binary_linear(coefficients, right);
    if (result.status == linearstatus::none) {
        cout << -1 << '\n';
        return 0;
    }
    cout << result.basis.size() << '\n';
    print_bits(result.solution);
    for (const auto& direction : result.basis) print_bits(direction);
}
