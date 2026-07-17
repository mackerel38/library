#include <cassert>
#include <random>
#include <vector>

#include "graph/cycle.hpp"

int main() {
    std::mt19937 random(411);
    for (int trial = 0; trial < 1000; ++trial) {
        const int vertices = 2 + random() % 7;
        const int edge_count = random() % 13;
        std::vector<std::pair<int, int>> edges;
        std::vector multiplicity(vertices, std::vector<int>(vertices));
        for (int edge = 0; edge < edge_count; ++edge) {
            int first = random() % vertices;
            int second = random() % vertices;
            if (first == second) second = (second + 1) % vertices;
            edges.emplace_back(first, second);
            ++multiplicity[first][second];
            ++multiplicity[second][first];
        }
        long long expected = 0;
        for (int mask = 0; mask < (1 << edge_count); ++mask) {
            if (std::popcount(static_cast<unsigned>(mask)) < 2) continue;
            std::vector<int> degree(vertices);
            std::vector<std::vector<int>> adjacency(vertices);
            for (int edge = 0; edge < edge_count; ++edge) {
                if (!(mask >> edge & 1)) continue;
                const auto [first, second] = edges[edge];
                ++degree[first];
                ++degree[second];
                adjacency[first].push_back(second);
                adjacency[second].push_back(first);
            }
            int start = -1;
            bool valid = true;
            for (int vertex = 0; vertex < vertices; ++vertex) {
                if (degree[vertex] != 0 && degree[vertex] != 2) valid = false;
                if (degree[vertex] != 0) start = vertex;
            }
            if (!valid) continue;
            std::vector<int> stack{start};
            std::vector<bool> seen(vertices);
            seen[start] = true;
            while (!stack.empty()) {
                const int vertex = stack.back();
                stack.pop_back();
                for (int next : adjacency[vertex]) {
                    if (!seen[next]) seen[next] = true, stack.push_back(next);
                }
            }
            for (int vertex = 0; vertex < vertices; ++vertex) {
                if (degree[vertex] != 0 && !seen[vertex]) valid = false;
            }
            expected += valid;
        }
        assert(poe::count_simple_cycles<long long>(multiplicity) == expected);
    }
}
