#define PROBLEM "https://atcoder.jp/contests/abc411/tasks/abc411_g"
#include <bits/stdc++.h>
#include "graph/cycle.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m;
    cin >> n >> m;
    vector multiplicity(n, vector<int>(n));
    while (m--) {
        int first, second;
        cin >> first >> second;
        --first;
        --second;
        ++multiplicity[first][second];
        ++multiplicity[second][first];
    }
    cout << count_simple_cycles<modint998244353>(multiplicity) << '\n';
}
