#define PROBLEM "https://atcoder.jp/contests/abc282/tasks/abc282_h"
#include <bits/stdc++.h>
#include "algorithm/subarrayminsum.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long limit;
    cin >> n >> limit;
    vector<long long> minimums(n), weights(n);
    for (auto& value : minimums) cin >> value;
    for (auto& value : weights) cin >> value;
    cout << count_subarrays_min_plus_sum_at_most(minimums, weights, limit) << '\n';
}
