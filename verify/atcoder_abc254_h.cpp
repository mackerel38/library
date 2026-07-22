#include <bits/stdc++.h>
#include "algorithm/multisettransform.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<unsigned> source(n), target(n);
    for (auto& value : source) cin >> value;
    for (auto& value : target) cin >> value;
    const auto answer = minimum_double_halve_multiset_operations(source, target);
    cout << (answer ? *answer : -1) << '\n';
}
