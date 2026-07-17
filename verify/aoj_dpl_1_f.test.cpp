#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/DPL_1_F"
#include <bits/stdc++.h>
#include "dp/knapsack.hpp"

using namespace std;
using namespace poe;

int main() {
    int item_count;
    long long capacity;
    cin >> item_count >> capacity;
    vector<long long> weights(item_count), values(item_count);
    for (int item = 0; item < item_count; ++item) cin >> values[item] >> weights[item];
    cout << knapsack01_auto(weights, values, capacity) << '\n';
}
