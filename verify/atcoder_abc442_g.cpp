#include <bits/stdc++.h>
#include "dp/knapsack.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long capacity;
    cin >> n >> capacity;
    vector<int> weight(n);
    vector<long long> value(n), count(n);
    for (int i = 0; i < n; ++i) cin >> weight[i] >> value[i] >> count[i];
    cout << bounded_knapsack_tiny_weights(weight, value, count, capacity) << '\n';
}
