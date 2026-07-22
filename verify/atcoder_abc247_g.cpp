#include <bits/stdc++.h>
#include "flow/matching.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<weightedbipartiteedge<long long>> edges(n);
    for (auto& [left, right, weight] : edges) {
        cin >> left >> right >> weight;
        --left;
        --right;
    }
    const auto answer = maximum_weight_bipartite_matching_values(150, 150, edges);
    cout << answer.size() - 1 << '\n';
    for (int size = 1; size < static_cast<int>(answer.size()); ++size) {
        cout << answer[size] << '\n';
    }
}
