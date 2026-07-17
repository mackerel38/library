#include <algorithm>
#include <cassert>
#include <queue>
#include <random>
#include <vector>

#include "recipe/treepathpartition.hpp"

int brute(const poe::undirected_graph<>& graph) {
    const int size = graph.size();
    int answer = size;
    std::vector<int> color(size);
    auto evaluate = [&] {
        const int colors = *std::max_element(color.begin(), color.end()) + 1;
        for (int current = 0; current < colors; ++current) {
            int start = -1;
            int count = 0;
            for (int vertex = 0; vertex < size; ++vertex) {
                if (color[vertex] == current) start = vertex, ++count;
            }
            std::queue<int> queue;
            std::vector<unsigned char> seen(size);
            queue.push(start);
            seen[start] = true;
            int reached = 0;
            while (!queue.empty()) {
                const int vertex = queue.front();
                queue.pop();
                ++reached;
                int degree = 0;
                for (const auto& edge : graph[vertex]) {
                    if (color[edge.to] != current) continue;
                    ++degree;
                    if (!seen[edge.to]) seen[edge.to] = true, queue.push(edge.to);
                }
                if (degree > 2) return;
            }
            if (reached != count) return;
        }
        int width = 1;
        for (int source = 0; source < size; ++source) {
            std::vector<int> parent(size, -1);
            std::queue<int> queue;
            parent[source] = source;
            queue.push(source);
            while (!queue.empty()) {
                const int vertex = queue.front();
                queue.pop();
                for (const auto& edge : graph[vertex]) {
                    if (parent[edge.to] != -1) continue;
                    parent[edge.to] = vertex;
                    queue.push(edge.to);
                }
            }
            for (int target = source; target < size; ++target) {
                std::vector<unsigned char> used(colors);
                for (int vertex = target;; vertex = parent[vertex]) {
                    used[color[vertex]] = true;
                    if (vertex == source) break;
                }
                width = std::max(width, std::accumulate(used.begin(), used.end(), 0));
            }
        }
        answer = std::min(answer, width);
    };
    auto search = [&](auto&& self, int vertex, int colors) -> void {
        if (vertex == size) {
            evaluate();
            return;
        }
        for (int current = 0; current <= colors; ++current) {
            color[vertex] = current;
            self(self, vertex + 1, std::max(colors, current + 1));
        }
    };
    color[0] = 0;
    search(search, 1, 1);
    return answer;
}

int main() {
    std::mt19937 random(20260716);
    for (int size = 1; size <= 8; ++size) {
        for (int trial = 0; trial < 20; ++trial) {
            poe::undirected_graph graph(size);
            for (int vertex = 1; vertex < size; ++vertex) {
                graph.add_edge(vertex, random() % vertex);
            }
            const int actual = poe::minimum_path_partition_width(graph);
            const int expected = brute(graph);
            if (actual != expected) {
                std::cerr << "size=" << size << " actual=" << actual
                          << " expected=" << expected << '\n';
                for (const auto& edge : graph.edges()) {
                    std::cerr << edge.from << ' ' << edge.to << '\n';
                }
                return 1;
            }
        }
    }
}
