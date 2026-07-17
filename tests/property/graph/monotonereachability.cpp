#include <cassert>
#include <queue>
#include <random>
#include "graph/monotonereachability.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int trial = 0; trial < 300; ++trial) {
        const int n = 1 + random() % 20;
        poe::directed_graph<> graph(n);
        for (int from = 0; from < n; ++from) for (int to = 0; to < n; ++to) {
            if (from != to && random() % 7 == 0) graph.add_edge(from, to);
        }
        poe::monotonereachability actual(graph);
        std::vector<int> sources;
        for (int query = 0; query < n * 2; ++query) {
            sources.push_back(random() % n);
            actual.add_source(sources.back());
            std::vector<bool> seen(n);
            std::queue<int> queue;
            for (int source : sources) if (!seen[source]) seen[source] = true, queue.push(source);
            while (!queue.empty()) {
                int vertex = queue.front(); queue.pop();
                for (const auto& edge : graph[vertex]) if (!seen[edge.to]) {
                    seen[edge.to] = true; queue.push(edge.to);
                }
            }
            for (int vertex = 0; vertex < n; ++vertex) assert(actual.contains(vertex) == seen[vertex]);
        }
    }
}
