#include <cassert>
#include <random>
#include <vector>

#include "graph/graph.hpp"
#include "graph/modularcomplete.hpp"
#include "graph/shortestpath.hpp"

using namespace poe;

int main() {
    std::mt19937 random(232);
    for (int trial = 0; trial < 5000; ++trial) {
        const int size = random() % 8 + 1;
        const long long modulus = random() % 30 + 1;
        std::vector<long long> outgoing(size), incoming(size);
        for (auto& value : outgoing) value = random() % modulus;
        for (auto& value : incoming) value = random() % modulus;
        const int start = random() % size;

        directed_graph<long long> graph(size);
        for (int from = 0; from < size; ++from) {
            for (int to = 0; to < size; ++to) {
                if (from != to) graph.add_edge(from, to, (outgoing[from] + incoming[to]) % modulus);
            }
        }
        assert(modular_complete_distances(outgoing, incoming, modulus, start) ==
               dijkstra(graph, start).distance);
    }
}
