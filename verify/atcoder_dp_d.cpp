#define PROBLEM "https://atcoder.jp/contests/dp/tasks/dp_d"
#include <bits/stdc++.h>
#include "dp/knapsack.hpp"

using namespace std;
using namespace poe;

int main() {
    int items, capacity;
    cin >> items >> capacity;
    vector<int> weight(items);
    vector<long long> value(items);
    for (int i = 0; i < items; ++i) cin >> weight[i] >> value[i];
    cout << knapsack01(weight, value, capacity) << '\n';
}
