#include <bits/stdc++.h>
#include "structure/excludedset.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> forbidden(n);
    for (auto& value : forbidden) cin >> value;
    excludedset data(std::move(forbidden));
    while (q--) {
        long long lower, rank;
        cin >> lower >> rank;
        cout << data.kth_allowed(lower, rank) << '\n';
    }
}
