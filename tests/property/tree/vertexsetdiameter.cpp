#include <algorithm>
#include <cassert>
#include <queue>
#include <random>
#include <vector>

#include "graph/graph.hpp"
#include "tree/vertexsetdiameter.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 300; ++trial) {
        const int n = 1 + random() % 18;
        poe::undirected_graph<> graph(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            graph.add_edge(vertex, random() % vertex);
        }
        std::vector distance(n, std::vector<int>(n, -1));
        for (int start = 0; start < n; ++start) {
            std::queue<int> queue;
            distance[start][start] = 0;
            queue.push(start);
            while (!queue.empty()) {
                const int vertex = queue.front();
                queue.pop();
                for (const auto& edge : graph[vertex]) {
                    if (distance[start][edge.to] != -1) continue;
                    distance[start][edge.to] = distance[start][vertex] + 1;
                    queue.push(edge.to);
                }
            }
        }
        std::vector<bool> active(n);
        for (int vertex = 0; vertex < n; ++vertex) active[vertex] = random() % 2;
        poe::vertexsetdiameter diameter(graph, active);
        for (int query = 0; query < 100; ++query) {
            const int vertex = random() % n;
            active[vertex] = !active[vertex];
            diameter.flip(vertex);
            int brute = 0;
            bool any = false;
            for (int left = 0; left < n; ++left) {
                if (!active[left]) continue;
                any = true;
                for (int right = 0; right < n; ++right) {
                    if (active[right]) brute = std::max(brute, distance[left][right]);
                }
            }
            assert(diameter.get().empty() == !any);
            if (any) assert(diameter.get().distance == brute);
        }
    }
}
