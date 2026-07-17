#include <algorithm>
#include <cassert>
#include <functional>
#include <random>
#include <vector>

#include "graph/maximumcut.hpp"

int main() {
    std::mt19937 random(427);
    for (int trial = 0; trial < 1000; ++trial) {
        const int size = random() % 12;
        poe::undirected_graph<int> graph(size);
        for (int left = 0; left < size; ++left) {
            for (int right = left + 1; right < size; ++right) {
                if (random() % 3 == 0) graph.add_edge(left, right, random() % 10);
            }
        }
        const auto result = poe::maximum_cut(graph);
        int actual = 0;
        for (const auto& edge : graph.edges()) {
            if (result.side[edge.from] != result.side[edge.to]) actual += edge.cost;
        }
        assert(actual == result.weight);
        int expected = 0;
        std::vector<bool> side(size);
        std::function<void(int)> enumerate = [&](int vertex) {
            if (vertex == size) {
                int weight = 0;
                for (const auto& edge : graph.edges()) {
                    if (side[edge.from] != side[edge.to]) weight += edge.cost;
                }
                expected = std::max(expected, weight);
                return;
            }
            side[vertex] = false;
            enumerate(vertex + 1);
            side[vertex] = true;
            enumerate(vertex + 1);
        };
        enumerate(0);
        assert(result.weight == expected);
    }
}
