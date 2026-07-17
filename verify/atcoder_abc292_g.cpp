#include <bits/stdc++.h>

#include "dp/wildcardorder.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<string> patterns(n);
    for (auto& pattern : patterns) cin >> pattern;
    cout << count_strictly_increasing_wildcard_numbers<998244353>(patterns) << '\n';
}
