#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "structure/fenwicktree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> permutation(n), before(n), after(n);
    for (int& value : permutation) cin >> value, --value;
    fenwicktree<int> count(n);
    for (int i = 0; i < n; ++i) {
        before[i] = count.sum(permutation[i]);
        count.add(permutation[i], 1);
    }
    count = fenwicktree<int>(n);
    for (int i = n - 1; i >= 0; --i) {
        after[i] = count.sum(permutation[i]);
        count.add(permutation[i], 1);
    }
    using mint = modint998244353;
    mint answer = 0;
    for (int i = 0; i < n; ++i) answer += mint{before[i]} * after[i];
    mint weighted_prefix = 0;
    for (int i = 0; i < n; ++i) {
        answer += weighted_prefix * after[i];
        weighted_prefix = 2 * weighted_prefix + before[i];
    }
    cout << answer.val() << '\n';
}
