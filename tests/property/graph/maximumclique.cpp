#include <algorithm>
#include <cassert>
#include <cstdint>
#include <random>
#include <vector>
#include "graph/graph.hpp"
#include "graph/maximumclique.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260715);
    for (int n = 0; n <= 16; ++n) {
        for (int repetition = 0; repetition < 60; ++repetition) {
            undirected_graph graph(n);
            vector<uint32_t> adjacent(n);
            for (int first = 0; first < n; ++first) {
                for (int second = first + 1; second < n; ++second) {
                    if (random() % 3 == 0) {
                        graph.add_edge(first, second);
                        adjacent[first] |= 1U << second;
                        adjacent[second] |= 1U << first;
                    }
                }
            }

            int clique_size = 0;
            int independent_size = 0;
            for (uint32_t mask = 0; mask < (1U << n); ++mask) {
                bool clique = true;
                bool independent = true;
                for (int vertex = 0; vertex < n; ++vertex) {
                    if (!(mask >> vertex & 1U)) continue;
                    const uint32_t others = mask ^ (1U << vertex);
                    clique &= (adjacent[vertex] & others) == others;
                    independent &= (adjacent[vertex] & others) == 0;
                }
                if (clique) clique_size = max(clique_size, popcount(mask));
                if (independent) independent_size = max(independent_size, popcount(mask));
            }
            assert(maximum_clique(graph).size() == clique_size);
            assert(maximum_independent_set(graph).size() == independent_size);
        }
    }
}
