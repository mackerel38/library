#define PROBLEM "https://atcoder.jp/contests/abc412/tasks/abc412_g"
#include <bits/stdc++.h>
#include "graph/generalmatching.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> limit(n);
    for (auto& value : limit) cin >> value;
    vector adjacency(n, vector<bool>(n));
    while (m--) {
        int first, second;
        cin >> first >> second;
        --first;
        --second;
        adjacency[first][second] = adjacency[second][first] = true;
    }

    vector<int> label;
    for (int vertex = 0; vertex < n; ++vertex) {
        label.insert(label.end(), limit[vertex], vertex);
    }
    vector<binarymatchingedge> edges;
    for (int first = 0; first < static_cast<int>(label.size()); ++first) {
        for (int second = first + 1; second < static_cast<int>(label.size()); ++second) {
            if (label[first] == label[second]) {
                edges.push_back({first, second, 0});
            } else if (adjacency[label[first]][label[second]]) {
                edges.push_back({first, second, 1});
            }
        }
    }
    const auto answer =
        minimum_binary_perfect_matching_weight(static_cast<int>(label.size()), edges);
    cout << (answer ? *answer : -1) << '\n';
}
