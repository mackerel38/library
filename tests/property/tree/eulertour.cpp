#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <vector>
#include "graph/graph.hpp"
#include "tree/eulertour.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260715);
    for (int n = 1; n <= 80; ++n) {
        for (int repetition = 0; repetition < 30; ++repetition) {
            undirected_graph graph(n);
            for (int vertex = 1; vertex < n; ++vertex) {
                graph.add_edge(uniform_int_distribution<int>(0, vertex - 1)(random), vertex);
            }
            const int root = uniform_int_distribution<int>(0, n - 1)(random);
            eulertour tour(graph, root);
            assert(tour.size() == 2 * n - 1);
            assert(static_cast<int>(tour.edges().size()) == 2 * n - 2);

            vector<int> first(n, -1), last(n, -1), appearances(n);
            for (int index = 0; index < tour.size(); ++index) {
                const int vertex = tour[index];
                if (first[vertex] == -1) first[vertex] = index;
                last[vertex] = index;
                ++appearances[vertex];
                assert(tour.depth(index) >= 0);
                if (index + 1 < tour.size()) {
                    const auto& edge = graph.edge_at(tour.edge(index));
                    const int next = tour[index + 1];
                    assert((edge.from == vertex && edge.to == next) ||
                           (edge.from == next && edge.to == vertex));
                    assert(abs(tour.depth(index) - tour.depth(index + 1)) == 1);
                }
            }
            for (int vertex = 0; vertex < n; ++vertex) {
                assert(tour.first(vertex) == first[vertex]);
                assert(tour.last(vertex) == last[vertex]);
                assert(appearances[vertex] == graph.degree(vertex) + (vertex == root));
                const auto [left, right] = tour.subtree(vertex);
                assert(left == tour.index(vertex));
                assert(right - left >= 1);
                for (int index = left; index < right; ++index) {
                    const int descendant = tour.preorder(index);
                    assert(tour.first(vertex) <= tour.first(descendant));
                    assert(tour.last(descendant) <= tour.last(vertex));
                }
            }
        }
    }
}
