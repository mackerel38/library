#define PROBLEM "https://judge.yosupo.jp/problem/two_sat"
#include <bits/stdc++.h>
#include "graph/twosat.hpp"

using namespace std;
using namespace poe;

int main() {
    string type;
    int variables, clauses;
    cin >> type >> type >> variables >> clauses;
    twosat solver(variables);
    for (int i = 0; i < clauses; ++i) {
        int left, right, zero;
        cin >> left >> right >> zero;
        const int left_variable = abs(left) - 1;
        const int right_variable = abs(right) - 1;
        solver.add_clause(left_variable, left > 0, right_variable, right > 0);
    }
    if (!solver.satisfiable()) {
        cout << "s UNSATISFIABLE\n";
        return 0;
    }
    cout << "s SATISFIABLE\nv";
    const auto answer = solver.answer();
    for (int i = 0; i < variables; ++i) cout << ' ' << (answer[i] ? i + 1 : -i - 1);
    cout << " 0\n";
}
