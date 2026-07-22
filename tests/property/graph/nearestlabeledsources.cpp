#include <bits/stdc++.h>
#include "graph/shortestpath.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(245);
    constexpr long long inf = numeric_limits<long long>::max();
    for (int trial = 0; trial < 3000; ++trial) {
        const int n = uniform_int_distribution<int>(1, 9)(random);
        undirected_graph<long long> graph(n);
        vector<vector<long long>> distance(n, vector<long long>(n, inf));
        for (int vertex = 0; vertex < n; ++vertex) distance[vertex][vertex] = 0;
        for (int left = 0; left < n; ++left) {
            for (int right = left + 1; right < n; ++right) {
                if (random() % 3 != 0) continue;
                const long long cost = random() % 20;
                graph.add_edge(left, right, cost);
                distance[left][right] = distance[right][left] = cost;
            }
        }
        for (int middle = 0; middle < n; ++middle) {
            for (int left = 0; left < n; ++left) {
                for (int right = 0; right < n; ++right) {
                    if (distance[left][middle] == inf || distance[middle][right] == inf) {
                        continue;
                    }
                    distance[left][right] = min(
                        distance[left][right],
                        distance[left][middle] + distance[middle][right]);
                }
            }
        }
        vector<int> labels(n), sources;
        for (int vertex = 0; vertex < n; ++vertex) {
            labels[vertex] = random() % 4;
            if (random() % 2) sources.push_back(vertex);
        }
        const auto actual = distance_to_nearest_different_label_source(
            graph, labels, sources);
        for (int vertex = 0; vertex < n; ++vertex) {
            long long expected = inf;
            for (const int source : sources) {
                if (labels[source] != labels[vertex]) {
                    expected = min(expected, distance[vertex][source]);
                }
            }
            assert(actual[vertex] == expected);
        }
    }
}
