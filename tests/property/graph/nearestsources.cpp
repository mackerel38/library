#include <algorithm>
#include <cassert>
#include <queue>
#include <random>
#include <vector>

#include "graph/shortestpath.hpp"

int main() {
    std::mt19937 random(429);
    for (int trial = 0; trial < 300; ++trial) {
        const int n = 2 + random() % 18;
        poe::undirected_graph graph(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            graph.add_edge(vertex, random() % vertex);
        }
        for (int edge = 0; edge < n; ++edge) {
            const int left = random() % n;
            const int right = random() % n;
            if (left != right) graph.add_edge(left, right);
        }
        std::vector<int> starts;
        for (int vertex = 0; vertex < n; ++vertex) {
            if (random() % 3 == 0) starts.push_back(vertex);
        }
        if (starts.empty()) starts.push_back(random() % n);
        const auto result = poe::nearest_sources_bfs<3>(graph, starts);
        std::vector<std::vector<int>> all_distance;
        for (const int start : starts) all_distance.push_back(poe::bfs(graph, start).distance);
        for (int vertex = 0; vertex < n; ++vertex) {
            std::vector<int> expected;
            for (const auto& distance : all_distance) expected.push_back(distance[vertex]);
            std::ranges::sort(expected);
            expected.resize(std::min<int>(3, expected.size()));
            for (int index = 0; index < 3; ++index) {
                if (index >= static_cast<int>(expected.size())) {
                    assert(!result.reachable(vertex, index));
                    continue;
                }
                assert(result.reachable(vertex, index));
                assert(result[vertex][index].distance == expected[index]);
                assert(all_distance[std::ranges::find(starts, result[vertex][index].source) - starts.begin()][vertex]
                       == result[vertex][index].distance);
                for (int previous = 0; previous < index; ++previous) {
                    assert(result[vertex][previous].source != result[vertex][index].source);
                }
            }
        }
    }
}
