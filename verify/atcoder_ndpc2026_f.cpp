#include <bits/stdc++.h>
#include "tree/cartesiantree.hpp"

using namespace std;
using namespace poe;


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    constexpr long long infinity = numeric_limits<long long>::max() / 4;
    while (test_count--) {
        int n;
        cin >> n;
        vector<int> permutation(n);
        vector<long long> cost(n);
        for (auto& x : permutation) cin >> x;
        for (auto& x : cost) cin >> x;
        const auto tree = cartesian_tree(permutation);
        vector<vector<long long>> dp(n);
        vector<pair<int, bool>> stack{{tree.root, false}};
        while (!stack.empty()) {
            const auto [vertex, visited] = stack.back();
            stack.pop_back();
            if (!visited) {
                stack.push_back({vertex, true});
                if (tree.right[vertex] != -1) stack.push_back({tree.right[vertex], false});
                if (tree.left[vertex] != -1) stack.push_back({tree.left[vertex], false});
                continue;
            }
            const vector<long long> empty{0};
            const auto& left = tree.left[vertex] == -1 ? empty : dp[tree.left[vertex]];
            const auto& right = tree.right[vertex] == -1 ? empty : dp[tree.right[vertex]];
            dp[vertex].assign(left.size() + right.size(), infinity);
            dp[vertex][0] = 0;
            for (int size = 1; size < static_cast<int>(left.size()); ++size) {
                dp[vertex][size] = min(dp[vertex][size], left[size]);
            }
            for (int size = 1; size < static_cast<int>(right.size()); ++size) {
                dp[vertex][size] = min(dp[vertex][size], right[size]);
            }
            for (int left_size = 0; left_size < static_cast<int>(left.size()); ++left_size) {
                for (int right_size = 0; right_size < static_cast<int>(right.size()); ++right_size) {
                    dp[vertex][left_size + right_size + 1] = min(
                        dp[vertex][left_size + right_size + 1],
                        left[left_size] + right[right_size] + cost[vertex]
                    );
                }
            }
        }
        for (int size = 1; size <= n; ++size) {
            cout << dp[tree.root][size] << (size == n ? '\n' : ' ');
        }
    }
}
