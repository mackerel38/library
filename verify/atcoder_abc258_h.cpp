#include <bits/stdc++.h>
#include "math/matrix.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    unsigned long long total;
    cin >> n >> total;
    vector<unsigned long long> forbidden(n);
    for (auto& value : forbidden) cin >> value;

    using mint = modint998244353;
    const matrix<mint> normal({{1, 0, 1}, {1, 0, 1}, {0, 1, 0}});
    const matrix<mint> blocked({{0, 0, 0}, {1, 0, 1}, {0, 1, 0}});
    const auto state = sparse_matrix_transitions(
        vector<mint>{1, 0, 0}, normal, blocked, total, forbidden
    );
    cout << state[0] << '\n';
}
