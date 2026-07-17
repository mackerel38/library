#include "graph/graph.hpp"
#include "tree/heavyrecursivedp.hpp"
#include "tree/rootedtree.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int n = 1; n <= 16; ++n) {
        for (int trial = 0; trial < 200; ++trial) {
            poe::undirected_graph graph(n);
            for (int vertex = 1; vertex < n; ++vertex) {
                graph.add_edge(vertex, random() % vertex);
            }
            std::vector<std::array<int, 4>> add(n);
            for (auto& values : add) {
                for (int& value : values) value = random() % 11;
            }
            poe::rootedtree tree(graph, 0);
            std::vector<std::vector<int>> children(n);
            for (int vertex = 0; vertex < n; ++vertex) {
                for (const auto& edge : graph[vertex]) {
                    if (tree.parent(edge.to) == vertex) children[vertex].push_back(edge.to);
                }
                std::ranges::sort(children[vertex], [&](int left, int right) {
                    return tree.subtree_size(left) > tree.subtree_size(right);
                });
            }
            const auto transition = [&](int vertex, auto& states) {
                const auto old = states;
                for (int branch = 0; branch < 4; ++branch) {
                    states[branch] = std::max(
                        old[branch] + add[vertex][branch],
                        old[(branch + 1) % 4] + add[vertex][branch] / 2);
                }
            };
            const auto brute = [&](this auto&& self, int vertex, int input)
                -> std::array<int, 4> {
                std::array<int, 4> states{input, input, input, input};
                for (const int child : children[vertex]) {
                    for (int branch = 0; branch < 4; ++branch) {
                        states[branch] = self(child, states[branch])[branch];
                    }
                }
                transition(vertex, states);
                return states;
            };
            std::vector<std::array<int, 4>> actual(n);
            const auto result = poe::heavyrecursivedp<4>(
                graph,
                0,
                7,
                transition,
                [&](int vertex, const auto& states) { actual[vertex] = states; });
            assert(result == brute(0, 7));
            for (int vertex = 0; vertex < n; ++vertex) {
                assert(actual[vertex] == brute(vertex, 7));
            }
        }
    }
}
