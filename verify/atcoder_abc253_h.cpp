#include <bits/stdc++.h>
#include "graph/spanningforest.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    matrix<modint998244353> multiplicity(n, n);
    for (int edge = 0; edge < m; ++edge) {
        int left, right;
        cin >> left >> right;
        --left;
        --right;
        ++multiplicity[left][right];
        ++multiplicity[right][left];
    }

    const auto forests = spanning_forest_counts(multiplicity);
    modint998244353 factorial = 1;
    modint998244353 inverse_power = 1;
    const modint998244353 inverse_edges = modint998244353(m).inv();
    for (int count = 1; count < n; ++count) {
        factorial *= count;
        inverse_power *= inverse_edges;
        cout << forests[count] * factorial * inverse_power << '\n';
    }
}
