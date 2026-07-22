#include <bits/stdc++.h>
#include "game/alternating.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(261);
    for (int trial = 0; trial < 500; ++trial) {
        const int n = 1 + random() % 5;
        directed_graph<int> graph(n);
        for (int from = 0; from < n; ++from) {
            vector<int> order(n);
            iota(order.begin(), order.end(), 0);
            shuffle(order.begin(), order.end(), random);
            const int degree = random() % 3;
            for (int index = 0; index < min(n, degree); ++index) {
                graph.add_edge(from, order[index], random() % 6);
            }
        }

        vector<vector<int>> policies(1, vector<int>(n, -1));
        for (int vertex = 0; vertex < n; ++vertex) {
            if (graph[vertex].empty()) continue;
            vector<vector<int>> next;
            for (const auto& policy : policies) {
                for (int edge = 0; edge < static_cast<int>(graph[vertex].size()); ++edge) {
                    auto extended = policy;
                    extended[vertex] = edge;
                    next.push_back(std::move(extended));
                }
            }
            policies = std::move(next);
        }

        const auto play = [&](int start, int first,
                              const vector<int>& minimize,
                              const vector<int>& maximize) -> optional<int> {
            vector<char> seen(2 * n);
            int vertex = start;
            int player = first;
            int value = 0;
            while (!graph[vertex].empty()) {
                const int state = 2 * vertex + player;
                if (seen[state]) return nullopt;
                seen[state] = true;
                const auto& policy = player == 0 ? minimize : maximize;
                const auto& edge = graph[vertex][policy[vertex]];
                value += edge.cost;
                vertex = edge.to;
                player ^= 1;
            }
            return value;
        };
        const auto greater_outcome = [](const optional<int>& left,
                                        const optional<int>& right) {
            if (!right) return static_cast<bool>(left);
            if (!left) return false;
            return *left < *right;
        };

        const auto result = alternating_path_game(graph);
        for (int start = 0; start < n; ++start) {
            for (int first = 0; first < 2; ++first) {
                optional<int> expected;
                bool has_minimum = false;
                for (const auto& minimize : policies) {
                    optional<int> worst = 0;
                    bool has_maximum = false;
                    for (const auto& maximize : policies) {
                        const auto outcome = play(start, first, minimize, maximize);
                        if (!has_maximum || greater_outcome(worst, outcome)) {
                            worst = outcome;
                            has_maximum = true;
                        }
                    }
                    if (!has_minimum || greater_outcome(worst, expected)) {
                        expected = worst;
                        has_minimum = true;
                    }
                }
                const auto player = static_cast<gameplayer>(first);
                assert(result.value(start, player) == expected);
            }
        }
    }
}
