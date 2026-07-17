#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, length;
    long long lower, upper;
    cin >> n >> m >> length >> lower >> upper;
    vector<vector<pair<int, long long>>> graph(n);
    while (m--) {
        int from, to;
        long long cost;
        cin >> from >> to >> cost;
        graph[--from].emplace_back(--to, cost);
    }
    vector<int> reachable(n);
    auto dfs = [&](auto&& self, int vertex, int depth, long long cost) -> void {
        if (cost > upper) return;
        if (depth == length) {
            if (cost >= lower) reachable[vertex] = 1;
            return;
        }
        for (const auto& [next, edge_cost] : graph[vertex]) {
            self(self, next, depth + 1, cost + edge_cost);
        }
    };
    dfs(dfs, 0, 0, 0);
    bool first = true;
    for (int vertex = 0; vertex < n; ++vertex) if (reachable[vertex]) {
        if (!first) cout << ' ';
        cout << vertex + 1;
        first = false;
    }
    cout << '\n';
}
