#include "graph/graph.hpp"
#include "graph/topologicalsort.hpp"

bool valid(
    const poe::directed_graph<>& graph,
    const std::vector<int>& lower,
    const std::vector<int>& upper,
    const std::vector<int>& position
) {
    for (int vertex = 0; vertex < graph.size(); ++vertex) {
        if (!(lower[vertex] <= position[vertex] && position[vertex] <= upper[vertex])) return false;
        for (const auto& edge : graph[vertex]) {
            if (position[vertex] >= position[edge.to]) return false;
        }
    }
    return true;
}

bool brute(
    const poe::directed_graph<>& graph,
    const std::vector<int>& lower,
    const std::vector<int>& upper
) {
    std::vector<int> order(graph.size());
    std::iota(order.begin(), order.end(), 0);
    do {
        std::vector<int> position(graph.size());
        for (int index = 0; index < graph.size(); ++index) position[order[index]] = index;
        if (valid(graph, lower, upper, position)) return true;
    } while (std::next_permutation(order.begin(), order.end()));
    return false;
}

int main() {
    std::mt19937 random(20260716);
    for (int n = 2; n <= 8; ++n) {
        const int trials = n <= 7 ? 300 : 30;
        for (int trial = 0; trial < trials; ++trial) {
            poe::directed_graph graph(n);
            for (int from = 0; from < n; ++from) {
                for (int to = from + 1; to < n; ++to) {
                    if (random() % 4 == 0) graph.add_edge(from, to);
                }
            }
            std::vector<int> lower(n), upper(n);
            for (int vertex = 0; vertex < n; ++vertex) {
                lower[vertex] = random() % n;
                upper[vertex] = random() % n;
                if (lower[vertex] > upper[vertex]) std::swap(lower[vertex], upper[vertex]);
            }
            const bool expected = brute(graph, lower, upper);
            const auto result = poe::bounded_topological_sort(graph, lower, upper);
            assert(result.found() == expected);
            if (result.found()) assert(valid(graph, lower, upper, result.position));
        }
    }
}
