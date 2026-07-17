#include <bits/stdc++.h>
#include "tree/cartesiantree.hpp"
using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> permutation(n), position(n + 1);
    for (int index = 0; index < n; ++index) {
        cin >> permutation[index];
        position[permutation[index]] = index;
    }
    const auto tree = cartesian_tree(permutation, greater{});
    vector<long long> dp(n);
    for (int value = 1; value <= n; ++value) {
        const int vertex = position[value];
        if (tree.left[vertex] != -1) {
            dp[vertex] = max(dp[vertex], dp[tree.left[vertex]] + vertex - tree.left[vertex]);
        }
        if (tree.right[vertex] != -1) {
            dp[vertex] = max(dp[vertex], dp[tree.right[vertex]] + tree.right[vertex] - vertex);
        }
    }
    cout << dp[tree.root] << '\n';
}
