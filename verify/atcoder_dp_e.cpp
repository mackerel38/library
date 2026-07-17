#define PROBLEM "https://atcoder.jp/contests/dp/tasks/dp_e"
#include <bits/stdc++.h>
#include "dp/knapsack.hpp"

using namespace std;
using namespace poe;

int main() {
    int items;
    long long capacity;
    cin >> items >> capacity;
    vector<long long> weight(items);
    vector<int> value(items);
    for (int i = 0; i < items; ++i) cin >> weight[i] >> value[i];
    const auto minimum_weight = knapsack01_by_value(weight, value);
    int answer = 0;
    for (int total = 0; total < static_cast<int>(minimum_weight.size()); ++total) {
        if (minimum_weight[total] <= capacity) answer = total;
    }
    cout << answer << '\n';
}
