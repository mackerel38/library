#include "tree/all.hpp"

struct distancevalue {
    long long count = 0;
    long long sum = 0;
};

int main() {
    std::mt19937 random(20260715);
    for (int n = 1; n <= 80; ++n) {
        for (int iteration = 0; iteration < 80; ++iteration) {
            poe::undirected_graph<long long> graph(n);
            std::vector<int> parent(n, -1), parent_edge(n, -1), depth(n);
            std::vector<long long> root_distance(n);
            for (int vertex = 1; vertex < n; ++vertex) {
                parent[vertex] = random() % vertex;
                const long long cost = random() % 11;
                parent_edge[vertex] = graph.add_edge(parent[vertex], vertex, cost);
                depth[vertex] = depth[parent[vertex]] + 1;
                root_distance[vertex] = root_distance[parent[vertex]] + cost;
            }
            std::vector<std::vector<long long>> distance(n, std::vector<long long>(n));
            for (int start = 0; start < n; ++start) {
                std::vector<int> previous(n, -1);
                std::vector<int> stack = {start};
                previous[start] = start;
                while (!stack.empty()) {
                    const int vertex = stack.back();
                    stack.pop_back();
                    for (const auto& current : graph[vertex]) {
                        if (previous[current.to] != -1) continue;
                        previous[current.to] = vertex;
                        distance[start][current.to] = distance[start][vertex] + current.cost;
                        stack.push_back(current.to);
                    }
                }
            }

            poe::rootedtree rooted(graph, 0);
            poe::lcatree ancestors(graph, 0);
            poe::hld decomposition(graph, 0);
            for (int vertex = 0; vertex < n; ++vertex) {
                assert(rooted.parent(vertex) == parent[vertex]);
                assert(rooted.parent_edge(vertex) == parent_edge[vertex]);
                assert(rooted.depth(vertex) == depth[vertex]);
                assert(rooted.distance(vertex) == root_distance[vertex]);
                assert(decomposition.vertex(decomposition[vertex]) == vertex);
                if (vertex != 0) {
                    assert(decomposition.edge_index(parent_edge[vertex]) ==
                           decomposition.index(vertex));
                }
            }

            for (int query = 0; query < 300; ++query) {
                const int left = random() % n;
                const int right = random() % n;
                int brute_left = left, brute_right = right;
                while (depth[brute_left] > depth[brute_right]) brute_left = parent[brute_left];
                while (depth[brute_right] > depth[brute_left]) brute_right = parent[brute_right];
                while (brute_left != brute_right) {
                    brute_left = parent[brute_left];
                    brute_right = parent[brute_right];
                }
                const int middle = brute_left;
                assert(ancestors.lca(left, right) == middle);
                assert(decomposition.lca(left, right) == middle);
                assert(ancestors.distance(left, right) == distance[left][right]);

                std::vector<int> upward;
                for (int vertex = left; vertex != middle; vertex = parent[vertex]) {
                    upward.push_back(vertex);
                }
                upward.push_back(middle);
                std::vector<int> downward;
                for (int vertex = right; vertex != middle; vertex = parent[vertex]) {
                    downward.push_back(vertex);
                }
                std::reverse(downward.begin(), downward.end());
                upward.insert(upward.end(), downward.begin(), downward.end());

                for (int vertex = 0; vertex < n; ++vertex) {
                    assert(ancestors.on_path(vertex, left, right) ==
                           (std::ranges::find(upward, vertex) != upward.end()));
                }
                if (left != right) {
                    const int neighbor = upward[1];
                    int expected_side = 0;
                    for (int vertex = 0; vertex < n; ++vertex) {
                        // left-neighbor辺を使わずleftから到達できる側。
                        const bool neighbor_path = std::ranges::find(
                            upward.begin(), upward.end(), vertex) != upward.end();
                        (void)neighbor_path;
                        expected_side += ancestors.jump(left, vertex, 1) != neighbor;
                    }
                    assert(ancestors.side_size(left, right) == expected_side);
                } else {
                    assert(ancestors.side_size(left, right) == n);
                }

                std::vector<int> restored;
                for (const auto& segment : decomposition.path(left, right)) {
                    if (segment.reversed) {
                        for (int index = segment.right; index-- > segment.left;) {
                            restored.push_back(decomposition.vertex(index));
                        }
                    } else {
                        for (int index = segment.left; index < segment.right; ++index) {
                            restored.push_back(decomposition.vertex(index));
                        }
                    }
                }
                assert(restored == upward);

                std::vector<int> restored_edges;
                for (const auto& segment : decomposition.edge_path(left, right)) {
                    if (segment.reversed) {
                        for (int index = segment.right; index-- > segment.left;) {
                            restored_edges.push_back(decomposition.vertex(index));
                        }
                    } else {
                        for (int index = segment.left; index < segment.right; ++index) {
                            restored_edges.push_back(decomposition.vertex(index));
                        }
                    }
                }
                auto expected_edges = upward;
                expected_edges.erase(std::ranges::find(expected_edges, middle));
                assert(restored_edges == expected_edges);
                for (int step = 0; step <= static_cast<int>(upward.size()); ++step) {
                    const int expected = step < static_cast<int>(upward.size()) ? upward[step] : -1;
                    assert(ancestors.jump(left, right, step) == expected);
                }
            }

            for (int root = 0; root < n; ++root) {
                const auto [left, right] = decomposition.subtree(root);
                for (int vertex = 0; vertex < n; ++vertex) {
                    const bool in_subtree = left <= decomposition.index(vertex) &&
                                            decomposition.index(vertex) < right;
                    assert(in_subtree == rooted.is_ancestor(root, vertex));
                }
            }

            const auto diameter = poe::tree_diameter(graph);
            long long expected_diameter = 0;
            for (const auto& row : distance) {
                expected_diameter = std::max(expected_diameter, *std::ranges::max_element(row));
            }
            assert(diameter.distance == expected_diameter);

            const auto rerooted = poe::rerooting(
                graph, distancevalue{},
                [](distancevalue left, distancevalue right) {
                    return distancevalue{left.count + right.count, left.sum + right.sum};
                },
                [](distancevalue value, int) {
                    ++value.count;
                    return value;
                },
                [](distancevalue value, const auto& current) {
                    value.sum += value.count * current.cost;
                    return value;
                });
            for (int root = 0; root < n; ++root) {
                assert(rerooted[root].count == n);
                assert(rerooted[root].sum ==
                       std::accumulate(distance[root].begin(), distance[root].end(), 0LL));
            }
        }
    }
}
