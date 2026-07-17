#include <cassert>
#include <random>
#include "graph/simplepath.hpp"

int main() {
    std::mt19937 random(712367821);
    for (int size = 2; size <= 8; ++size) {
        for (int iteration = 0; iteration < 500; ++iteration) {
            poe::undirected_graph<> graph(size);
            for (int left = 0; left < size; ++left) {
                for (int right = left + 1; right < size; ++right) {
                    if (random() % 4 == 0) graph.add_edge(left, right);
                }
            }
            std::vector<long long> expected(size);
            std::vector<char> visited(size);
            auto dfs = [&](auto&& self, int vertex, int length) -> void {
                if (vertex == size - 1) {
                    ++expected[length];
                    return;
                }
                visited[vertex] = true;
                for (const auto& current : graph[vertex]) {
                    if (!visited[current.to]) self(self, current.to, length + 1);
                }
                visited[vertex] = false;
            };
            dfs(dfs, 0, 0);
            assert(poe::simple_path_length_counts(graph, 0, size - 1) == expected);
        }
    }
}
