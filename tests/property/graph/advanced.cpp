#include "graph/cycle.hpp"
#include "graph/topologicalsort.hpp"
#include "graph/warshallfloyd.hpp"
#include "structure/dsu.hpp"

constexpr long long infinity = (1LL << 50);

int main() {
    std::mt19937 random(20260716);
    for (int n = 1; n <= 12; ++n) {
        for (int iteration = 0; iteration < 300; ++iteration) {
            poe::directed_graph<> graph(n);
            const int m = random() % (n * n + 1);
            for (int index = 0; index < m; ++index) {
                graph.add_edge(random() % n, random() % n);
            }
            const auto cycle = poe::find_cycle(graph);
            assert(cycle.found() == !poe::topological_sort(graph).is_dag());
            if (cycle.found()) {
                assert(cycle.vertices.size() == cycle.edge_ids.size());
                for (int index = 0; index < static_cast<int>(cycle.vertices.size()); ++index) {
                    const auto& current = graph.edge_at(cycle.edge_ids[index]);
                    assert(current.from == cycle.vertices[index]);
                    assert(current.to == cycle.vertices[(index + 1) % cycle.vertices.size()]);
                }
            }
        }
    }

    for (int n = 1; n <= 12; ++n) {
        for (int iteration = 0; iteration < 300; ++iteration) {
            poe::undirected_graph<> graph(n);
            poe::dsu components(n);
            bool expected = false;
            const int m = random() % (n * n + 1);
            for (int index = 0; index < m; ++index) {
                const int left = random() % n;
                const int right = random() % n;
                expected |= components.same(left, right);
                components.merge(left, right);
                graph.add_edge(left, right);
            }
            const auto cycle = poe::find_cycle(graph);
            assert(cycle.found() == expected);
            if (cycle.found()) {
                assert(cycle.vertices.size() == cycle.edge_ids.size());
                for (int index = 0; index < static_cast<int>(cycle.vertices.size()); ++index) {
                    const auto& current = graph.edge_at(cycle.edge_ids[index]);
                    const int from = cycle.vertices[index];
                    const int to = cycle.vertices[(index + 1) % cycle.vertices.size()];
                    assert((current.from == from && current.to == to) ||
                           (current.from == to && current.to == from));
                }
            }
        }
    }

    for (int n = 1; n <= 8; ++n) {
        for (int iteration = 0; iteration < 250; ++iteration) {
            poe::directed_graph<long long> graph(n);
            std::vector<std::vector<long long>> expected(n, std::vector<long long>(n, infinity));
            for (int vertex = 0; vertex < n; ++vertex) expected[vertex][vertex] = 0;
            const int m = random() % (n * n + 1);
            for (int index = 0; index < m; ++index) {
                const int from = random() % n;
                const int to = random() % n;
                const long long cost = static_cast<int>(random() % 13) - 4;
                graph.add_edge(from, to, cost);
                expected[from][to] = std::min(expected[from][to], cost);
            }
            for (int middle = 0; middle < n; ++middle)
                for (int from = 0; from < n; ++from)
                    for (int to = 0; to < n; ++to)
                        if (expected[from][middle] < infinity && expected[middle][to] < infinity)
                            expected[from][to] = std::min(
                                expected[from][to], expected[from][middle] + expected[middle][to]);
            const auto result = poe::warshall_floyd(graph);
            for (int from = 0; from < n; ++from) {
                for (int to = 0; to < n; ++to) {
                    assert(result.reachable(from, to) == (expected[from][to] < infinity));
                    if (result.reachable(from, to)) assert(result(from, to) == expected[from][to]);
                    bool negative = false;
                    for (int middle = 0; middle < n; ++middle) {
                        negative |= expected[from][middle] < infinity &&
                                    expected[middle][middle] < 0 &&
                                    expected[middle][to] < infinity;
                    }
                    assert(result.affected_by_negative_cycle(from, to) == negative);
                }
            }
        }
    }
}
