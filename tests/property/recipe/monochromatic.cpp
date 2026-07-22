#include <cassert>
#include <queue>
#include <random>
#include <vector>

#include "graph/graph.hpp"
#include "recipe/monochromatic.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 250; ++trial) {
        const int n = 1 + random() % 20;
        poe::undirected_graph<> graph(n);
        for (int vertex = 1; vertex < n; ++vertex) graph.add_edge(vertex, random() % vertex);
        std::vector<long long> weight(n);
        std::vector<int> color(n);
        for (auto& value : weight) value = random() % 30;
        for (int& value : color) value = random() % 2;
        poe::monochromatictree data(graph, weight, color);
        for (int query = 0; query < 200; ++query) {
            const int vertex = random() % n;
            const int type = random() % 4;
            if (type == 0) {
                color[vertex] ^= 1;
                data.flip(vertex);
            } else if (type == 1) {
                const long long delta = static_cast<int>(random() % 21) - 10;
                weight[vertex] += delta;
                data.add(vertex, delta);
            } else if (type == 2) {
                weight[vertex] = static_cast<int>(random() % 51) - 20;
                data.set(vertex, weight[vertex]);
            } else {
                long long brute = 0;
                std::vector<bool> seen(n);
                std::queue<int> queue;
                seen[vertex] = true;
                queue.push(vertex);
                while (!queue.empty()) {
                    const int current = queue.front();
                    queue.pop();
                    brute += weight[current];
                    for (const auto& edge : graph[current]) {
                        if (seen[edge.to] || color[edge.to] != color[vertex]) continue;
                        seen[edge.to] = true;
                        queue.push(edge.to);
                    }
                }
                assert(data.sum(vertex) == brute);
                assert(data.color(vertex) == color[vertex] && data[vertex] == weight[vertex]);
            }
        }
    }
}
