#define PROBLEM "https://atcoder.jp/contests/abc426/tasks/abc426_g"
#include <bits/stdc++.h>
#include "dp/rangeknapsack.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<int> weights(n);
    vector<long long> values(n);
    for (int item = 0; item < n; ++item) cin >> weights[item] >> values[item];
    int query_count;
    cin >> query_count;
    vector<rangeknapsackquery> queries(query_count);
    for (auto& [left, right, capacity] : queries) {
        cin >> left >> right >> capacity;
        --left;
    }
    for (const long long answer : range_knapsack01(weights, values, queries)) cout << answer << '\n';
}
