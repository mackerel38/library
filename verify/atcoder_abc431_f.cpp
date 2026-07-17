#include <bits/stdc++.h>
#include "algorithm/permutation.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long maximum_drop;
    cin >> n >> maximum_drop;
    vector<long long> values(n);
    for (long long& value : values) cin >> value;
    cout << count_permutations_max_drop<998244353>(values, maximum_drop) << '\n';
}
