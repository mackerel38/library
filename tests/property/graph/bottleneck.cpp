#include "graph/bottleneck.hpp"
#include "graph/graph.hpp"

using namespace std;
using namespace poe;

vector<vector<int>> minimax(const undirected_graph<int>& graph, int raised_edge = -1) {
    const int n = graph.size();
    const int infinity = 1'000'000'000;
    vector distance(n, vector<int>(n, infinity));
    for (int vertex = 0; vertex < n; ++vertex) distance[vertex][vertex] = 0;
    for (const auto& edge : graph.edges()) {
        const int cost = edge.cost + (edge.id == raised_edge);
        distance[edge.from][edge.to] = min(distance[edge.from][edge.to], cost);
        distance[edge.to][edge.from] = min(distance[edge.to][edge.from], cost);
    }
    for (int middle = 0; middle < n; ++middle) {
        for (int from = 0; from < n; ++from) {
            for (int to = 0; to < n; ++to) {
                distance[from][to] = min(
                    distance[from][to],
                    max(distance[from][middle], distance[middle][to])
                );
            }
        }
    }
    return distance;
}

int main() {
    mt19937 random(20260716);
    for (int trial = 0; trial < 2000; ++trial) {
        const int n = 2 + random() % 8;
        undirected_graph<int> graph(n);
        vector distance(n, vector<int>(n, 1'000'000));
        for (int vertex = 0; vertex < n; ++vertex) distance[vertex][vertex] = 0;
        for (int vertex = 1; vertex < n; ++vertex) {
            const int parent = random() % vertex;
            const int cost = 1 + random() % 9;
            graph.add_edge(vertex, parent, cost);
            distance[vertex][parent] = distance[parent][vertex] = cost;
        }
        for (int edge = 0; edge < 5; ++edge) {
            const int first = random() % n;
            const int second = random() % n;
            if (first == second) continue;
            const int cost = 1 + random() % 9;
            graph.add_edge(first, second, cost);
            distance[first][second] = distance[second][first]
                = min(distance[first][second], cost);
        }
        for (int middle = 0; middle < n; ++middle)
            for (int first = 0; first < n; ++first)
                for (int second = 0; second < n; ++second)
                    distance[first][second] = min(distance[first][second],
                        distance[first][middle] + distance[middle][second]);

        vector<int> terminals;
        for (int vertex = 0; vertex < n; ++vertex)
            if (random() % 2) terminals.push_back(vertex);
        if (terminals.empty()) terminals.push_back(random() % n);
        vector<terminalhopquery<int>> queries;
        for (int query = 0; query < 30; ++query) {
            queries.push_back({terminals[random() % terminals.size()],
                               terminals[random() % terminals.size()],
                               static_cast<int>(random() % 31)});
        }
        const auto answer = terminal_hop_connectivity(graph, terminals, queries);
        for (int query = 0; query < static_cast<int>(queries.size()); ++query) {
            vector<char> seen(n);
            queue<int> queue;
            seen[queries[query].from] = true;
            queue.push(queries[query].from);
            while (!queue.empty()) {
                const int current = queue.front();
                queue.pop();
                for (const int next : terminals) {
                    if (!seen[next] && distance[current][next] <= queries[query].limit) {
                        seen[next] = true;
                        queue.push(next);
                    }
                }
            }
            assert(answer[query] == static_cast<bool>(seen[queries[query].to]));
        }
    }
    for (int size = 1; size <= 14; ++size) {
        for (int trial = 0; trial < 100; ++trial) {
            directed_graph<> graph(size);
            for (int from = 0; from < size; ++from) {
                for (int to = 0; to < size; ++to) {
                    if (from != to && random() % 4 == 0) graph.add_edge(from, to);
                }
            }
            vector<int> cost(size);
            for (int& value : cost) value = random() % 7;
            constexpr int infinity = 1 << 20;
            vector distance(size, vector<int>(size, infinity));
            for (int vertex = 0; vertex < size; ++vertex) distance[vertex][vertex] = cost[vertex];
            for (int from = 0; from < size; ++from) {
                for (const auto& edge : graph[from]) {
                    distance[from][edge.to] = min(distance[from][edge.to],
                        max(cost[from], cost[edge.to]));
                }
            }
            for (int middle = 0; middle < size; ++middle) {
                for (int from = 0; from < size; ++from) {
                    for (int to = 0; to < size; ++to) {
                        distance[from][to] = min(distance[from][to],
                            max(distance[from][middle], distance[middle][to]));
                    }
                }
            }
            vector<pair<int, int>> queries;
            for (int from = 0; from < size; ++from) {
                for (int to = 0; to < size; ++to) queries.emplace_back(from, to);
            }
            const auto answer = minimum_vertex_bottleneck_paths(graph, cost, queries);
            for (int query = 0; query < static_cast<int>(queries.size()); ++query) {
                const auto [from, to] = queries[query];
                if (distance[from][to] == infinity) assert(!answer[query].has_value());
                else assert(answer[query] == distance[from][to]);
            }
        }
    }
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = 2 + random() % 7;
        undirected_graph<int> graph(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            graph.add_edge(vertex, random() % vertex, 1 + random() % 5);
        }
        const int extra = random() % 8;
        for (int edge = 0; edge < extra; ++edge) {
            int left = random() % n;
            int right = random() % n;
            if (left == right && random() % 2) right = (right + 1) % n;
            graph.add_edge(left, right, 1 + random() % 5);
        }
        const auto original = minimax(graph);
        bottlenecksensitivity sensitivity(graph);
        for (int edge = 0; edge < graph.edge_count(); ++edge) {
            const auto raised = minimax(graph, edge);
            for (int first = 0; first < n; ++first) {
                for (int second = first + 1; second < n; ++second) {
                    const bool expected = original[first][second] < raised[first][second];
                    assert(sensitivity.increases(edge, first, second) == expected);
                }
            }
        }
    }
}
