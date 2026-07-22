#include <bits/stdc++.h>
#include "algorithm/subarrayrange.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    cin >> n >> k;
    vector<int> permutation(n);
    for (int& value : permutation) cin >> value;
    cout << count_subarrays_range_excess_at_most(permutation, k) << '\n';
}
