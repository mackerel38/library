#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "tree/pathfold.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> values(n);
    for (int& value : values) cin >> value;
    undirected_graph graph(n);
    graph.read(n - 1);

    const auto stats = tree_path_fold_statistics(
        graph, values, [](int left, int right) { return gcd(left, right); });
    modint998244353 answer = 0;
    for (const auto& [value, state] : stats) {
        answer += modint998244353(value) * state.length_sum;
    }
    cout << answer << '\n';
}
