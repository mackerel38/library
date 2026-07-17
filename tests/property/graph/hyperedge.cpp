#include <cassert>
#include <queue>
#include <random>
#include <vector>
#include "graph/graph.hpp"
#include "graph/hyperedge.hpp"
int main() {
    std::mt19937 random(436);
    for (int n = 1; n <= 30; ++n) for (int trial = 0; trial < 100; ++trial) {
        poe::undirected_graph<> graph(n);
        std::vector<std::vector<int>> explicit_graph(n);
        for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) if (random() % 8 == 0) {
            graph.add_edge(u, v); explicit_graph[u].push_back(v); explicit_graph[v].push_back(u);
        }
        std::vector<std::vector<int>> groups(random() % 8);
        for (auto& group : groups) for (int v = 0; v < n; ++v) if (random() % 4 == 0) group.push_back(v);
        for (const auto& group : groups) for (int u : group) for (int v : group) if (u != v) explicit_graph[u].push_back(v);
        const int start = random() % n;
        std::vector<int> expected(n, -1); expected[start] = 0; std::queue<int> queue; queue.push(start);
        while (!queue.empty()) { int v=queue.front();queue.pop();for(int to:explicit_graph[v])if(expected[to]<0)expected[to]=expected[v]+1,queue.push(to); }
        assert(poe::bfs_with_hyperedges(graph, groups, start) == expected);
    }
}
