#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/DPL_1_C"
#include <bits/stdc++.h>
#include "dp/knapsack.hpp"

using namespace std;
using namespace poe;

int main() {
    int item_count, capacity;
    cin >> item_count >> capacity;
    vector<int> weights(item_count);
    vector<long long> values(item_count);
    for (int item = 0; item < item_count; ++item) cin >> values[item] >> weights[item];
    cout << unbounded_knapsack(weights, values, capacity) << '\n';
}
