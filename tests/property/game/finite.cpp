#include <cassert>
#include <functional>
#include <random>
#include <vector>

#include "game/finite.hpp"

int main() {
    std::mt19937 random(427);
    for (int trial = 0; trial < 2000; ++trial) {
        const int size = 1 + random() % 8;
        poe::directed_graph graph(size);
        for (int from = 0; from < size; ++from) {
            for (int to = 0; to < size; ++to) {
                if (random() % 4 == 0) graph.add_edge(from, to);
            }
        }
        std::vector<bool> terminal(size);
        for (int vertex = 0; vertex < size; ++vertex) terminal[vertex] = random() & 1;
        const int turns = random() % 8;
        const bool first_maximizes = random() & 1;
        const auto result = poe::finite_horizon_game(
            graph, terminal, turns, first_maximizes
        );
        std::vector memo(turns + 1, std::vector<int>(size, -1));
        std::function<bool(int, int)> solve = [&](int turn, int vertex) {
            if (turn == turns) return static_cast<bool>(terminal[vertex]);
            if (memo[turn][vertex] != -1) return static_cast<bool>(memo[turn][vertex]);
            const bool maximize = first_maximizes ^ static_cast<bool>(turn & 1);
            bool answer = !maximize;
            for (const auto& edge : graph[vertex]) {
                if (maximize) answer = answer || solve(turn + 1, edge.to);
                else answer = answer && solve(turn + 1, edge.to);
            }
            memo[turn][vertex] = answer;
            return answer;
        };
        for (int vertex = 0; vertex < size; ++vertex) {
            assert(result[vertex] == solve(0, vertex));
        }
    }
}
