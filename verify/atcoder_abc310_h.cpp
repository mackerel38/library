#define PROBLEM "https://atcoder.jp/contests/abc310/tasks/abc310_h"
#include <bits/stdc++.h>
#include "dp/knapsack.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    long long target;
    cin >> n >> target;
    constexpr int limit = 300;
    vector<int> increase(n);
    vector<long long> damage(n);
    for (int item = 0; item < n; ++item) {
        int cost;
        cin >> cost >> damage[item];
        increase[item] = -cost;
    }

    constexpr long long unreachable = -1;
    vector<long long> dp(2 * limit, unreachable);
    dp[0] = 0;
    vector<int> costs;
    vector<long long> values;
    for (int length = 1; length <= 2 * limit; ++length) {
        vector<long long> next(2 * limit, unreachable);
        for (int balance = 0; balance < 2 * limit; ++balance) {
            if (dp[balance] == unreachable) continue;
            for (int item = 0; item < n; ++item) {
                const int next_balance = balance + increase[item];
                if (next_balance < 0 || next_balance >= 2 * limit) continue;
                next[next_balance] = max(
                    next[next_balance], dp[balance] + damage[item]);
            }
        }
        dp = std::move(next);
        const long long best = *max_element(dp.begin(), dp.end());
        if (best != unreachable) {
            costs.push_back(length);
            values.push_back(best);
        }
    }
    cout << minimum_unbounded_knapsack_cost(costs, values, target) << '\n';
}

