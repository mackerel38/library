#include <bits/stdc++.h>
#include "dp/segments.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    long long base = 0;
    vector<long long> gain(n);
    for (int i = 0; i < n; ++i) {
        long long front, back;
        cin >> front >> back;
        base += front;
        gain[i] = back - front;
    }
    cout << base + maximum_k_disjoint_subarray_sum(gain, k) << '\n';
}
