#include <algorithm>
#include <cassert>
#include <queue>
#include <random>
#include <vector>

#include "graph/graph.hpp"
#include "tree/constrainedpath.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int n = 1; n <= 30; ++n) {
        for (int repetition = 0; repetition < 100; ++repetition) {
            poe::undirected_graph<> graph(n);
            for (int vertex = 1; vertex < n; ++vertex)
                graph.add_edge(vertex, random() % vertex);
            std::vector<bool> endpoint(n), internal(n);
            for (int vertex = 0; vertex < n; ++vertex) {
                endpoint[vertex] = random() % 2;
                internal[vertex] = random() % 2;
            }
            const int result = poe::longest_constrained_path(
                graph,
                [&](int vertex) { return endpoint[vertex]; },
                [&](int vertex) { return internal[vertex]; }
            );
            int brute = 0;
            for (int source = 0; source < n; ++source) {
                std::vector<int> parent(n, -2);
                std::queue<int> queue;
                parent[source] = -1;
                queue.push(source);
                while (!queue.empty()) {
                    const int vertex = queue.front();
                    queue.pop();
                    for (const auto& edge : graph[vertex]) {
                        if (parent[edge.to] != -2) continue;
                        parent[edge.to] = vertex;
                        queue.push(edge.to);
                    }
                }
                for (int target = source; target < n; ++target) {
                    if (!endpoint[source] || !endpoint[target]) continue;
                    int length = 1;
                    bool valid = true;
                    for (int vertex = target; vertex != source; vertex = parent[vertex]) {
                        ++length;
                        if (parent[vertex] != source && !internal[parent[vertex]]) valid = false;
                    }
                    if (valid) brute = std::max(brute, length);
                }
            }
            assert(result == brute);
        }
    }
}
