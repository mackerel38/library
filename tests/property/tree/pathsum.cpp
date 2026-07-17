#include <cassert>
#include <queue>
#include <random>
#include <vector>

#include "tree/pathsum.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int size = 1; size <= 40; ++size) {
        poe::undirected_graph<long long> graph(size);
        std::vector<long long> weight;
        for (int vertex = 1; vertex < size; ++vertex) {
            graph.add_edge(vertex, random() % vertex, random() % 20);
            weight.push_back(graph.edge_at(vertex - 1).cost);
        }
        poe::edgepathsum data(graph);
        for (int query = 0; query < 200; ++query) {
            if (size > 1 && random() % 2 == 0) {
                const int id = random() % (size - 1);
                weight[id] = random() % 30;
                data.set(id, weight[id]);
                assert(data[id] == weight[id]);
                continue;
            }
            const int source = random() % size;
            const int target = random() % size;
            std::vector<int> parent(size, -1), parent_edge(size, -1);
            std::queue<int> queue;
            parent[source] = source;
            queue.push(source);
            while (!queue.empty()) {
                const int vertex = queue.front();
                queue.pop();
                for (const auto& edge : graph[vertex]) {
                    if (parent[edge.to] != -1) continue;
                    parent[edge.to] = vertex;
                    parent_edge[edge.to] = edge.id;
                    queue.push(edge.to);
                }
            }
            long long expected = 0;
            for (int vertex = target; vertex != source; vertex = parent[vertex]) {
                expected += weight[parent_edge[vertex]];
            }
            assert(data.sum(source, target) == expected);
        }
    }
}
