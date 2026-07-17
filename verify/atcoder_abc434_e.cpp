#include <bits/stdc++.h>
#include "graph/connectivity.hpp"
using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<pair<long long, long long>> edges(n);
    vector<long long> coordinates;
    coordinates.reserve(2 * n);
    for (auto& [left, right] : edges) {
        long long position, jump;
        cin >> position >> jump;
        left = position - jump;
        right = position + jump;
        coordinates.push_back(left);
        coordinates.push_back(right);
    }
    ranges::sort(coordinates);
    coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
    undirected_graph<> graph(coordinates.size());
    for (const auto& [left, right] : edges) {
        graph.add_edge(
            ranges::lower_bound(coordinates, left) - coordinates.begin(),
            ranges::lower_bound(coordinates, right) - coordinates.begin());
    }
    const auto components = connected_components(graph);
    int answer = 0;
    for (int component = 0; component < components.count; ++component) {
        answer += min(static_cast<int>(components.groups[component].size()),
                      components.edge_count[component]);
    }
    cout << answer << '\n';
}
