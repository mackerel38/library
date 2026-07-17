#include <cassert>
#include <random>
#include <vector>

#include "graph/graph.hpp"
#include "tree/dp.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 300; ++trial) {
        const int n = 1 + random() % 100;
        poe::undirected_graph<> graph(n);
        std::vector<int> parent(n, -1);
        for (int vertex = 1; vertex < n; ++vertex) {
            parent[vertex] = random() % vertex;
            graph.add_edge(parent[vertex], vertex);
        }
        const auto answer = poe::treedp(
            graph, 0, 0,
            [](int left, int right) { return left + right; },
            [](int children, int) { return children + 1; },
            [](int child, const auto&) { return child; }
        );
        std::vector<int> brute(n, 1);
        for (int vertex = n - 1; vertex > 0; --vertex) brute[parent[vertex]] += brute[vertex];
        assert(answer == brute);
    }
}
