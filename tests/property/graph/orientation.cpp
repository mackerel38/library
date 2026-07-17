#include "graph/graph.hpp"
#include "graph/orientation.hpp"

long long brute(const poe::undirected_graph<>& graph) {
    long long answer = 0;
    long long states = 1;
    for (int edge = 0; edge < graph.edge_count(); ++edge) states *= 3;
    for (long long code = 0; code < states; ++code) {
        long long current = code;
        std::vector<int> parent(graph.size());
        std::iota(parent.begin(), parent.end(), 0);
        const auto find = [&](this auto&& self, int vertex) -> int {
            return parent[vertex] == vertex
                ? vertex : parent[vertex] = self(parent[vertex]);
        };
        std::vector<int> choice(graph.edge_count());
        for (int edge = 0; edge < graph.edge_count(); ++edge) {
            choice[edge] = current % 3;
            current /= 3;
            if (choice[edge] == 2) {
                const auto& value = graph.edge_at(edge);
                parent[find(value.from)] = find(value.to);
            }
        }
        std::vector<std::vector<int>> directed(graph.size());
        std::vector<int> indegree(graph.size());
        bool valid = true;
        for (int edge = 0; edge < graph.edge_count(); ++edge) {
            if (choice[edge] == 2) continue;
            const auto& value = graph.edge_at(edge);
            int from = find(choice[edge] == 0 ? value.from : value.to);
            int to = find(choice[edge] == 0 ? value.to : value.from);
            if (from == to) {
                valid = false;
                break;
            }
            directed[from].push_back(to);
            ++indegree[to];
        }
        if (!valid) continue;
        std::queue<int> queue;
        int components = 0, removed = 0;
        for (int vertex = 0; vertex < graph.size(); ++vertex) {
            if (find(vertex) != vertex) continue;
            ++components;
            if (indegree[vertex] == 0) queue.push(vertex);
        }
        while (!queue.empty()) {
            const int vertex = queue.front();
            queue.pop();
            ++removed;
            for (const int to : directed[vertex]) {
                if (--indegree[to] == 0) queue.push(to);
            }
        }
        answer += removed == components;
    }
    return answer;
}

int main() {
    std::mt19937 random(20260716);
    for (int n = 2; n <= 6; ++n) {
        for (int trial = 0; trial < 300; ++trial) {
            poe::undirected_graph graph(n);
            for (int left = 0; left < n; ++left) {
                for (int right = left + 1; right < n; ++right) {
                    if (random() % 2 == 0 && graph.edge_count() < 9) {
                        graph.add_edge(left, right);
                    }
                }
            }
            assert(poe::count_acyclic_edge_states<long long>(graph) == brute(graph));
        }
    }
}

