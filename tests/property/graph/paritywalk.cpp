#include <bits/stdc++.h>
#include "graph/paritywalk.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260722);
    for (int trial = 0; trial < 5000; ++trial) {
        const int n = uniform_int_distribution<int>(1, 100)(random);
        undirected_graph graph(n);
        vector<vector<bool>> adjacent(n, vector<bool>(n));
        for (int vertex = 1; vertex < n; ++vertex) {
            const int parent = uniform_int_distribution<int>(0, vertex - 1)(random);
            graph.add_edge(parent, vertex);
            adjacent[parent][vertex] = adjacent[vertex][parent] = true;
        }
        for (int edge = 0; edge < n; ++edge) {
            const int left = random() % n, right = random() % n;
            if (left != right && !adjacent[left][right]) {
                graph.add_edge(left, right);
                adjacent[left][right] = adjacent[right][left] = true;
            }
        }
        vector<int> target(n);
        for (int& value : target) value = random() & 1;
        const int root = random() % n;
        const auto walk = vertex_parity_walk(graph, target, root);
        vector<int> actual(n);
        for (int index = 0; index < static_cast<int>(walk.size()); ++index) {
            assert(0 <= walk[index] && walk[index] < n);
            actual[walk[index]] ^= 1;
            if (index > 0) assert(adjacent[walk[index - 1]][walk[index]]);
        }
        assert(actual == target);
        assert(static_cast<int>(walk.size()) < 4 * n);
    }
}
