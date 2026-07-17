#include <bits/stdc++.h>
#include "algorithm/permutation.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> values(n);
    for (int& value : values) cin >> value;
    vector<long long> ranks(q);
    for (long long& rank : ranks) {
        cin >> rank;
        --rank;
    }
    for (const auto [left, right] : one_swap_lexicographic_pairs(values, ranks)) {
        cout << left + 1 << ' ' << right + 1 << '\n';
    }
}
