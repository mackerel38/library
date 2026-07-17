#include <cassert>
#include <random>
#include <vector>

#include "graph/coloring.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int size = 0; size <= 7; ++size) {
        for (int trial = 0; trial < 20; ++trial) {
            poe::undirected_graph graph(size);
            for (int left = 0; left < size; ++left) {
                for (int right = left + 1; right < size; ++right) {
                    if (random() % 3 == 0) graph.add_edge(left, right);
                }
            }
            const int colors = random() % 4;
            long long expected = 0;
            long long total = 1;
            for (int vertex = 0; vertex < size; ++vertex) total *= colors;
            std::vector<int> color(size);
            for (long long code = 0; code < total; ++code) {
                long long value = code;
                for (int vertex = 0; vertex < size; ++vertex) {
                    color[vertex] = value % colors;
                    value /= colors;
                }
                bool valid = true;
                for (const auto& edge : graph.edges()) {
                    valid &= color[edge.from] != color[edge.to];
                }
                expected += valid;
            }
            assert(poe::count_graph_colorings<998244353>(graph, colors).val() == expected);
        }
    }
}
