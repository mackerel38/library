#include <cassert>
#include <queue>
#include <random>
#include <vector>

#include "tree/diameter.hpp"

int main() {
    std::mt19937 random(428);
    for (int trial = 0; trial < 500; ++trial) {
        const int size = 1 + random() % 40;
        poe::undirected_graph graph(size);
        for (int vertex = 1; vertex < size; ++vertex) graph.add_edge(vertex, random() % vertex);
        const auto result = poe::tree_farthest_vertices(graph);
        for (int start = 0; start < size; ++start) {
            std::vector<int> distance(size, -1);
            std::queue<int> queue;
            distance[start] = 0;
            queue.push(start);
            while (!queue.empty()) {
                const int vertex = queue.front();
                queue.pop();
                for (const auto& edge : graph[vertex]) {
                    if (distance[edge.to] != -1) continue;
                    distance[edge.to] = distance[vertex] + 1;
                    queue.push(edge.to);
                }
            }
            int expected = 0;
            for (int vertex = 1; vertex < size; ++vertex) {
                if (std::pair{distance[vertex], vertex} > std::pair{distance[expected], expected}) {
                    expected = vertex;
                }
            }
            assert(result[start].vertex == expected);
            assert(result[start].distance == distance[expected]);
        }
    }
}
