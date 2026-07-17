#include <bits/stdc++.h>

#include "algorithm/twopointer.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    long long k;
    cin >> n >> k;
    vector<int> p(n);
    for (int& value : p) cin >> value;
    cout << count_subarrays_inversions(p, k) << '\n';
}
