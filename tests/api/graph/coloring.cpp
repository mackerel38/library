#include <cassert>

#include "graph/coloring.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    {
        poe::undirected_graph graph(4);
        assert(poe::count_graph_colorings<998244353>(graph, 10) == mint{10000});
    }
    {
        poe::undirected_graph graph(4);
        graph.add_edge(0, 1);
        graph.add_edge(1, 2);
        graph.add_edge(1, 3);
        assert(poe::count_graph_colorings<998244353>(graph, 2) == mint{2});
    }
    {
        poe::undirected_graph graph(5);
        for (int left = 0; left < 5; ++left) {
            for (int right = left + 1; right < 5; ++right) graph.add_edge(left, right);
        }
        assert(poe::count_graph_colorings<998244353>(graph, 5) == mint{120});
    }
}
