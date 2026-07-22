#define PROBLEM "https://atcoder.jp/contests/abc275/tasks/abc275_h"
#include <bits/stdc++.h>
#include "dp/decreasingconvex.hpp"
#include "tree/cartesiantree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> stamina(n), barrier(n);
    for (auto& value : stamina) cin >> value;
    for (auto& value : barrier) cin >> value;

    const auto tree = cartesian_tree(barrier, greater{});
    vector<int> order{tree.root};
    for (int index = 0; index < static_cast<int>(order.size()); ++index) {
        const int vertex = order[index];
        if (tree.left[vertex] != -1) order.push_back(tree.left[vertex]);
        if (tree.right[vertex] != -1) order.push_back(tree.right[vertex]);
    }
    vector<decreasingconvexsequence<long long>> dp(n);
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int vertex = *iterator;
        if (tree.left[vertex] != -1) {
            dp[vertex].merge(std::move(dp[tree.left[vertex]]));
        }
        if (tree.right[vertex] != -1) {
            dp[vertex].merge(std::move(dp[tree.right[vertex]]));
        }
        dp[vertex].suffix_min(barrier[vertex], stamina[vertex]);
    }
    cout << dp[tree.root].zero() << '\n';
}
