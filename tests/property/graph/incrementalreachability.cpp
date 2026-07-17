#include <algorithm>
#include <cassert>
#include <queue>
#include <random>
#include <vector>

#include "graph/graph.hpp"
#include "graph/incrementalreachability.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int n = 1; n <= 40; ++n) {
        for (int repetition = 0; repetition < 100; ++repetition) {
            poe::directed_graph<> graph(n);
            for (int i = 0; i < n * 3; ++i)
                graph.add_edge(random() % n, random() % n);
            std::vector<int> order(n);
            std::iota(order.begin(), order.end(), 0);
            std::shuffle(order.begin(), order.end(), random);
            const int source = random() % n;
            const auto result = poe::incremental_reachability(
                graph, order, std::vector<int>{source}
            );
            std::vector<bool> active(n);
            for (int step = 0; step < n; ++step) {
                active[order[step]] = true;
                std::vector<bool> reached(n);
                std::queue<int> queue;
                if (active[source]) {
                    reached[source] = true;
                    queue.push(source);
                }
                while (!queue.empty()) {
                    const int vertex = queue.front();
                    queue.pop();
                    for (const auto& edge : graph[vertex]) {
                        if (active[edge.to] && !reached[edge.to]) {
                            reached[edge.to] = true;
                            queue.push(edge.to);
                        }
                    }
                }
                assert(result.count[step] == std::ranges::count(reached, true));
                for (int vertex = 0; vertex < n; ++vertex) {
                    assert((result.reached_at[vertex] != -1
                            && result.reached_at[vertex] <= step) == reached[vertex]);
                }
            }
        }
    }
}
