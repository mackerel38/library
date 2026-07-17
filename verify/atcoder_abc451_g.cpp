#include <bits/stdc++.h>

#include "graph/graph.hpp"
#include "math/xorbasis.hpp"
#include "structure/xortrie.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n, m;
        unsigned k;
        cin >> n >> m >> k;
        undirected_graph<unsigned> graph(n);
        for (int i = 0; i < m; ++i) {
            int u, v;
            unsigned weight;
            cin >> u >> v >> weight;
            graph.add_edge(--u, --v, weight);
        }
        vector<unsigned> path(n);
        vector<int> parent(n, -2);
        parent[0] = -1;
        vector<int> stack{0};
        while (!stack.empty()) {
            int vertex = stack.back();
            stack.pop_back();
            for (const auto& edge : graph[vertex]) {
                if (parent[edge.to] != -2) continue;
                parent[edge.to] = vertex;
                path[edge.to] = path[vertex] ^ edge.cost;
                stack.push_back(edge.to);
            }
        }
        xorbasis<unsigned> basis;
        for (const auto& edge : graph.edges())
            basis.add(path[edge.from] ^ path[edge.to] ^ edge.cost);
        for (auto& value : path) value = basis.min(value);
        cout << count_pairs_xor_at_most<unsigned, 30>(path, k) << '\n';
    }
}
