#include <bits/stdc++.h>
#include "math/combination.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int x1, x2, x3;
    cin >> x1 >> x2 >> x3;
    const int total = x1 + x2 + x3;
    combination<modint998244353> comb(total);
    auto partition = [&](int count, int groups) {
        return comb.choose(count - 1, groups - 1);
    };

    modint998244353 answer = 0;
    for (int groups = 2; groups <= x1; ++groups) {
        answer += partition(x1, groups) * partition(x3, groups - 1) *
                  comb.choose(total - (2 * groups - 2), x1 + x3);
    }
    for (int groups = 2; groups <= x3; ++groups) {
        answer += partition(x3, groups) * partition(x1, groups - 1) *
                  comb.choose(total - (2 * groups - 2), x1 + x3);
    }
    for (int groups = 1; groups <= min(x1, x3); ++groups) {
        answer += modint998244353{2} * partition(x1, groups) * partition(x3, groups) *
                  comb.choose(total - (2 * groups - 1), x1 + x3);
    }
    cout << answer << '\n';
}
