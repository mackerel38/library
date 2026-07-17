#include "graph/graph.hpp"
#include "tree/treedistancesum.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260716);
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = 1 + random() % 60;
        undirected_graph graph(n);
        vector<int> parent(n, -1);
        for (int vertex = 1; vertex < n; ++vertex) {
            parent[vertex] = random() % vertex;
            graph.add_edge(parent[vertex], vertex);
        }
        vector distance(n, vector<int>(n, n));
        for (int source = 0; source < n; ++source) {
            queue<int> queue;
            distance[source][source] = 0;
            queue.push(source);
            while (!queue.empty()) {
                const int vertex = queue.front();
                queue.pop();
                for (const auto& edge : graph[vertex]) {
                    if (distance[source][edge.to] != n) continue;
                    distance[source][edge.to] = distance[source][vertex] + 1;
                    queue.push(edge.to);
                }
            }
        }
        vector<int> in(n), out(n);
        int timer = 0;
        const auto dfs = [&](this auto&& self, int vertex) -> void {
            in[vertex] = timer++;
            for (const auto& edge : graph[vertex]) {
                if (edge.to != parent[vertex]) self(edge.to);
            }
            out[vertex] = timer;
        };
        dfs(0);
        treedistancesum sums(graph);
        for (int source = 0; source < n; ++source) {
            assert(sums.sum(source)
                   == accumulate(distance[source].begin(), distance[source].end(), 0LL));
            for (int root = 0; root < n; ++root) {
                long long expected = 0;
                for (int vertex = 0; vertex < n; ++vertex) {
                    if (in[root] <= in[vertex] && in[vertex] < out[root]) {
                        expected += distance[source][vertex];
                    }
                }
                assert(sums.subtree_sum(source, root) == expected);
            }
            for (int other = 0; other < n; ++other) {
                long long expected = 0;
                for (int vertex = 0; vertex < n; ++vertex) {
                    expected += min(distance[source][vertex], distance[other][vertex]);
                }
                assert(sums.min_sum(source, other) == expected);
            }
        }
    }
}
