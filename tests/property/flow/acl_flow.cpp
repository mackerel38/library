#include "flow/maxflow.hpp"
#include "flow/mincostflow.hpp"

struct input_edge {
    int from;
    int to;
    int cap;
    int cost;
};

std::pair<int, int> reference_mincost(int n, const std::vector<input_edge>& input,
                                      int source, int sink, int limit) {
    struct edge {
        int to;
        int reverse;
        int cap;
        int cost;
    };
    std::vector<std::vector<edge>> graph(n);
    for (const auto& original : input) {
        const int from_index = graph[original.from].size();
        const int to_index = graph[original.to].size();
        graph[original.from].push_back({original.to, to_index, original.cap, original.cost});
        graph[original.to].push_back({original.from, from_index, 0, -original.cost});
    }
    int flow = 0;
    int cost = 0;
    while (flow < limit) {
        std::vector<int> distance(n, 1'000'000'000), previous_vertex(n), previous_edge(n);
        std::vector<bool> in_queue(n);
        std::queue<int> queue;
        distance[source] = 0;
        queue.push(source);
        in_queue[source] = true;
        while (!queue.empty()) {
            const int vertex = queue.front();
            queue.pop();
            in_queue[vertex] = false;
            for (int i = 0; i < static_cast<int>(graph[vertex].size()); ++i) {
                const edge& current = graph[vertex][i];
                if (current.cap > 0 &&
                    distance[current.to] > distance[vertex] + current.cost) {
                    distance[current.to] = distance[vertex] + current.cost;
                    previous_vertex[current.to] = vertex;
                    previous_edge[current.to] = i;
                    if (!in_queue[current.to]) {
                        in_queue[current.to] = true;
                        queue.push(current.to);
                    }
                }
            }
        }
        if (distance[sink] == 1'000'000'000) {
            break;
        }
        int add = limit - flow;
        for (int vertex = sink; vertex != source; vertex = previous_vertex[vertex]) {
            add = std::min(add, graph[previous_vertex[vertex]][previous_edge[vertex]].cap);
        }
        for (int vertex = sink; vertex != source; vertex = previous_vertex[vertex]) {
            edge& current = graph[previous_vertex[vertex]][previous_edge[vertex]];
            current.cap -= add;
            graph[vertex][current.reverse].cap += add;
        }
        flow += add;
        cost += add * distance[sink];
    }
    return {flow, cost};
}

int main() {
    std::mt19937 random(112233);
    for (int n = 2; n <= 8; ++n) {
        for (int iteration = 0; iteration < 500; ++iteration) {
            std::vector<input_edge> edges;
            poe::maxflow<int> graph(n);
            for (int from = 0; from < n; ++from) {
                for (int to = 0; to < n; ++to) {
                    if (from != to && random() % 5 == 0) {
                        const int cap = random() % 5;
                        graph.add_edge(from, to, cap);
                        edges.push_back({from, to, cap, 0});
                    }
                }
            }
            int expected = 1'000'000'000;
            for (int mask = 0; mask < (1 << n); ++mask) {
                if (!(mask & 1) || (mask & (1 << (n - 1)))) {
                    continue;
                }
                int cut = 0;
                for (const auto& current : edges) {
                    if ((mask >> current.from & 1) && !(mask >> current.to & 1)) {
                        cut += current.cap;
                    }
                }
                expected = std::min(expected, cut);
            }
            assert(graph.flow(0, n - 1) == expected);
        }
    }

    for (int n = 2; n <= 9; ++n) {
        for (int iteration = 0; iteration < 500; ++iteration) {
            std::vector<input_edge> edges;
            poe::mincostflow<int, int> graph(n);
            for (int from = 0; from < n; ++from) {
                for (int to = from + 1; to < n; ++to) {
                    if (random() % 3 == 0) {
                        input_edge current{from, to, static_cast<int>(random() % 4),
                                           static_cast<int>(random() % 8)};
                        edges.push_back(current);
                        graph.add_edge(current.from, current.to, current.cap, current.cost);
                    }
                }
            }
            const int limit = random() % 8;
            assert(graph.flow(0, n - 1, limit) ==
                   reference_mincost(n, edges, 0, n - 1, limit));
        }
    }
}
