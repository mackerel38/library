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
