#include <cassert>
#include <random>
#include <set>
#include "game/dag.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int n = 1; n <= 50; ++n) {
        for (int trial = 0; trial < 100; ++trial) {
            poe::directed_graph<> graph(n);
            for (int from = 0; from < n; ++from) for (int to = from + 1; to < n; ++to) {
                if (random() % 7 == 0) graph.add_edge(from, to);
            }
            std::vector<int> expected(n);
            for (int vertex = n - 1; vertex >= 0; --vertex) {
                std::set<int> next;
                for (const auto& edge : graph[vertex]) next.insert(expected[edge.to]);
                while (next.contains(expected[vertex])) ++expected[vertex];
            }
            assert(poe::dag_grundy(graph) == expected);
            const auto winning = poe::dag_winning_positions(graph);
            for (int vertex = 0; vertex < n; ++vertex) assert(winning[vertex] == (expected[vertex] != 0));
        }
    }
}
