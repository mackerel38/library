#include "graph/all.hpp"

constexpr long long inf = (1LL << 50);

int component_count_without(const poe::undirected_graph<>& graph, int removed_vertex,
                            int removed_edge) {
    std::vector<char> used(graph.size());
    int count = 0;
    for (int start = 0; start < graph.size(); ++start) {
        if (start == removed_vertex || used[start]) continue;
        ++count;
        used[start] = true;
        std::vector<int> stack = {start};
        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            for (const auto& current : graph[vertex]) {
                if (current.id == removed_edge || current.to == removed_vertex ||
                    used[current.to]) {
                    continue;
                }
                used[current.to] = true;
                stack.push_back(current.to);
            }
        }
    }
    return count;
}

long long prim_forest_cost(const poe::undirected_graph<long long>& graph) {
    using state = std::pair<long long, int>;
    std::vector<char> used(graph.size());
    long long result = 0;
    for (int start = 0; start < graph.size(); ++start) {
        if (used[start]) continue;
        std::priority_queue<state, std::vector<state>, std::greater<state>> queue;
        queue.emplace(0, start);
        while (!queue.empty()) {
            const auto [cost, vertex] = queue.top();
            queue.pop();
            if (used[vertex]) continue;
            used[vertex] = true;
            result += cost;
            for (const auto& current : graph[vertex]) {
                if (!used[current.to]) queue.emplace(current.cost, current.to);
            }
        }
    }
    return result;
}

int main() {
    std::mt19937 random(20260714);

    for (int n = 1; n <= 9; ++n) {
        for (int iteration = 0; iteration < 250; ++iteration) {
            poe::directed_graph<long long> graph(n);
            std::vector<std::vector<long long>> distance(n, std::vector<long long>(n, inf));
            for (int vertex = 0; vertex < n; ++vertex) distance[vertex][vertex] = 0;
            const int m = random() % (n * n + 1);
            for (int index = 0; index < m; ++index) {
                const int from = random() % n;
                const int to = random() % n;
                const long long cost = random() % 20;
                graph.add_edge(from, to, cost);
                distance[from][to] = std::min(distance[from][to], cost);
            }
            for (int middle = 0; middle < n; ++middle)
                for (int from = 0; from < n; ++from)
                    for (int to = 0; to < n; ++to)
                        if (distance[from][middle] < inf && distance[middle][to] < inf)
                            distance[from][to] = std::min(
                                distance[from][to], distance[from][middle] + distance[middle][to]);
            for (int start = 0; start < n; ++start) {
                const auto result = poe::dijkstra(graph, start);
                for (int vertex = 0; vertex < n; ++vertex) {
                    assert(result.reachable(vertex) == (distance[start][vertex] < inf));
                    if (result.reachable(vertex)) assert(result[vertex] == distance[start][vertex]);
                }
            }
        }
    }

    for (int n = 1; n <= 8; ++n) {
        for (int iteration = 0; iteration < 200; ++iteration) {
            poe::directed_graph<int> graph(n);
            const int m = random() % (n * n + 1);
            for (int index = 0; index < m; ++index) {
                graph.add_edge(random() % n, random() % n, random() & 1);
            }
            const auto binary = poe::zero_one_bfs(graph, 0);
            const auto implicit = poe::zero_one_bfs(n, 0, [&](int vertex, auto relax) {
                for (const auto& edge : graph[vertex]) relax(edge.to, edge.cost);
            });
            const auto heap = poe::dijkstra(graph, 0);
            assert(binary.distance == heap.distance);
            assert(implicit.distance == heap.distance);
        }
    }

    for (int n = 1; n <= 7; ++n) {
        for (int iteration = 0; iteration < 250; ++iteration) {
            poe::directed_graph<long long> graph(n);
            std::vector<std::vector<long long>> distance(n, std::vector<long long>(n, inf));
            for (int vertex = 0; vertex < n; ++vertex) distance[vertex][vertex] = 0;
            const int m = random() % (n * n + 1);
            for (int index = 0; index < m; ++index) {
                const int from = random() % n;
                const int to = random() % n;
                const long long cost = static_cast<int>(random() % 9) - 4;
                graph.add_edge(from, to, cost);
                distance[from][to] = std::min(distance[from][to], cost);
            }
            for (int middle = 0; middle < n; ++middle)
                for (int from = 0; from < n; ++from)
                    for (int to = 0; to < n; ++to)
                        if (distance[from][middle] < inf && distance[middle][to] < inf)
                            distance[from][to] = std::min(
                                distance[from][to], distance[from][middle] + distance[middle][to]);
            const auto result = poe::bellman_ford(graph, 0);
            for (int vertex = 0; vertex < n; ++vertex) {
                bool negative = false;
                for (int middle = 0; middle < n; ++middle) {
                    negative |= distance[0][middle] < inf && distance[middle][middle] < 0 &&
                                distance[middle][vertex] < inf;
                }
                assert(result.affected_by_negative_cycle(vertex) == negative);
                if (!negative && distance[0][vertex] < inf) {
                    assert(result[vertex] == distance[0][vertex]);
                }
            }
        }
    }

    for (int n = 0; n <= 10; ++n) {
        for (int iteration = 0; iteration < 250; ++iteration) {
            poe::undirected_graph<> graph(n);
            if (n > 0) {
                const int m = random() % (n * n + 1);
                for (int index = 0; index < m; ++index) {
                    graph.add_edge(random() % n, random() % n);
                }
            }
            const int original_count = component_count_without(graph, -1, -1);
            const auto components = poe::connected_components(graph);
            assert(components.count == original_count);
            std::vector<int> expected_edge_count(components.count);
            for (const auto& edge : graph.edges()) ++expected_edge_count[components[edge.from]];
            assert(components.edge_count == expected_edge_count);
            for (int component = 0; component < components.count; ++component) {
                assert(components.is_tree(component) ==
                       (expected_edge_count[component] + 1 ==
                        static_cast<int>(components.groups[component].size())));
            }
            const auto links = poe::lowlink(graph);
            std::vector<int> expected_articulation;
            std::vector<int> expected_bridges;
            for (int vertex = 0; vertex < n; ++vertex) {
                if (component_count_without(graph, vertex, -1) > original_count) {
                    expected_articulation.push_back(vertex);
                }
            }
            for (int id = 0; id < graph.edge_count(); ++id) {
                if (component_count_without(graph, -1, id) > original_count) {
                    expected_bridges.push_back(id);
                }
            }
            assert(links.articulation_points == expected_articulation);
            assert(links.bridge_ids == expected_bridges);
            const auto blocks = poe::two_edge_connected_components(graph);
            for (int left = 0; left < n; ++left) {
                for (int right = 0; right < n; ++right) {
                    bool reachable = false;
                    std::vector<char> used(n);
                    std::vector<int> stack = {left};
                    used[left] = true;
                    while (!stack.empty()) {
                        const int vertex = stack.back();
                        stack.pop_back();
                        if (vertex == right) reachable = true;
                        for (const auto& current : graph[vertex]) {
                            if (links.is_bridge(current.id) || used[current.to]) continue;
                            used[current.to] = true;
                            stack.push_back(current.to);
                        }
                    }
                    assert(blocks.same(left, right) == reachable);
                }
            }
        }
    }

    for (int n = 1; n <= 10; ++n) {
        for (int iteration = 0; iteration < 250; ++iteration) {
            poe::undirected_graph<long long> graph(n);
            const int m = random() % (n * n + 1);
            for (int index = 0; index < m; ++index) {
                graph.add_edge(random() % n, random() % n,
                               static_cast<int>(random() % 31) - 15);
            }
            const auto result = poe::minimum_spanning_forest(graph);
            assert(result.cost == prim_forest_cost(graph));
            assert(static_cast<int>(result.edge_ids.size()) == n - result.component_count);
        }
    }

    for (int n = 0; n <= 9; ++n) {
        for (int iteration = 0; iteration < 200; ++iteration) {
            poe::directed_graph<> graph(n);
            std::vector<int> permutation(n);
            std::iota(permutation.begin(), permutation.end(), 0);
            std::ranges::shuffle(permutation, random);
            std::vector<int> position(n);
            for (int index = 0; index < n; ++index) position[permutation[index]] = index;
            for (int from = 0; from < n; ++from) {
                for (int to = 0; to < n; ++to) {
                    if (position[from] < position[to] && random() % 4 == 0) {
                        graph.add_edge(from, to);
                    }
                }
            }
            const auto result = poe::topological_sort(graph);
            const auto minimum = poe::topological_sort_min(graph);
            assert(result.is_dag() && minimum.is_dag());
            std::vector<int> result_position(n), minimum_position(n);
            for (int index = 0; index < n; ++index) {
                result_position[result.order[index]] = index;
                minimum_position[minimum.order[index]] = index;
            }
            for (const auto& current : graph.edges()) {
                assert(result_position[current.from] < result_position[current.to]);
                assert(minimum_position[current.from] < minimum_position[current.to]);
            }
            std::vector<int> ways(1 << n);
            ways[0] = 1;
            for (int mask = 0; mask < (1 << n); ++mask) {
                if (ways[mask] == 0) continue;
                for (int vertex = 0; vertex < n; ++vertex) {
                    if (mask >> vertex & 1) continue;
                    bool ready = true;
                    for (const auto& current : graph.edges()) {
                        if (current.to == vertex && !(mask >> current.from & 1)) ready = false;
                    }
                    if (ready) {
                        ways[mask | (1 << vertex)] =
                            std::min(2, ways[mask | (1 << vertex)] + ways[mask]);
                    }
                }
            }
            assert(result.is_unique() == (ways.back() == 1));
            assert(minimum.is_unique() == (ways.back() == 1));
        }
    }

    for (int n = 1; n <= 8; ++n) {
        for (int iteration = 0; iteration < 200; ++iteration) {
            poe::undirected_graph<> graph(n);
            const int m = random() % (n * n + 1);
            for (int index = 0; index < m; ++index) {
                graph.add_edge(random() % n, random() % n);
            }
            bool expected = false;
            for (int mask = 0; mask < (1 << n); ++mask) {
                bool valid = true;
                for (const auto& current : graph.edges()) {
                    valid &= ((mask >> current.from) & 1) != ((mask >> current.to) & 1);
                }
                expected |= valid;
            }
            const auto result = poe::bipartite_coloring(graph);
            assert(result.is_bipartite() == expected);
            assert(poe::is_bipartite(graph) == expected);
            if (expected) {
                for (const auto& current : graph.edges()) {
                    assert(result[current.from] != result[current.to]);
                }
            }
        }
    }

    poe::directed_graph<> cycle(3);
    cycle.add_edge(0, 1);
    cycle.add_edge(1, 2);
    cycle.add_edge(2, 0);
    assert(!poe::topological_sort(cycle).is_dag());

    for (int n = 1; n <= 10; ++n) {
        for (int iteration = 0; iteration < 200; ++iteration) {
            poe::directed_graph<> graph(n);
            std::vector<std::vector<char>> reachable(n, std::vector<char>(n));
            for (int vertex = 0; vertex < n; ++vertex) reachable[vertex][vertex] = true;
            const int m = random() % (n * n + 1);
            for (int index = 0; index < m; ++index) {
                const int from = random() % n;
                const int to = random() % n;
                graph.add_edge(from, to);
                reachable[from][to] = true;
            }
            for (int middle = 0; middle < n; ++middle)
                for (int from = 0; from < n; ++from)
                    for (int to = 0; to < n; ++to)
                        reachable[from][to] |= reachable[from][middle] && reachable[middle][to];
            const auto [count, id] = poe::scc_ids(graph);
            assert(1 <= count && count <= n);
            for (int left = 0; left < n; ++left) {
                for (int right = 0; right < n; ++right) {
                    assert((id[left] == id[right]) ==
                           (reachable[left][right] && reachable[right][left]));
                }
            }
        }
    }
}
