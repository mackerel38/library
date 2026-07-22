#include <bits/stdc++.h>
#include "math/xorbasis.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    unsigned k;
    cin >> n >> k;
    vector<pair<unsigned, unsigned>> cards(n);
    for (auto& [front, back] : cards) cin >> front >> back;
    const auto answer = maximum_paired_xor_under<unsigned, 30>(cards, k);
    if (answer) cout << *answer << '\n';
    else cout << -1 << '\n';
}
