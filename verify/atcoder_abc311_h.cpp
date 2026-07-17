#define PROBLEM "https://atcoder.jp/contests/abc311/tasks/abc311_h"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "tree/heavyrecursivedp.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, capacity;
    cin >> n >> capacity;
    undirected_graph graph(n);
    for (int vertex = 1; vertex < n; ++vertex) {
        int parent;
        cin >> parent;
        graph.add_edge(parent - 1, vertex);
    }
    vector<long long> beauty(n);
    vector<int> weight(n), color(n);
    for (int vertex = 0; vertex < n; ++vertex) {
        cin >> beauty[vertex] >> weight[vertex] >> color[vertex];
    }

    constexpr long long negative = numeric_limits<long long>::lowest() / 4;
    vector<long long> initial(capacity + 1, negative);
    initial[0] = 0;
    vector<long long> answer(n);
    heavyrecursivedp<2>(
        graph,
        0,
        initial,
        [&](int vertex, auto& dp) {
            for (int used = 0; used + weight[vertex] <= capacity; ++used) {
                if (dp[color[vertex] ^ 1][used] == negative) continue;
                dp[color[vertex]][used + weight[vertex]] = max(
                    dp[color[vertex]][used + weight[vertex]],
                    dp[color[vertex] ^ 1][used] + beauty[vertex]);
            }
        },
        [&](int vertex, const auto& dp) {
            for (int used = 0; used + weight[vertex] <= capacity; ++used) {
                if (dp[color[vertex] ^ 1][used] == negative) continue;
                answer[vertex] = max(
                    answer[vertex],
                    dp[color[vertex] ^ 1][used] + beauty[vertex]);
            }
        });
    for (const long long value : answer) cout << value << '\n';
}

