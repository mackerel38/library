#include "graph/graph.hpp"
#include "graph/orientation.hpp"
#include "math/modint.hpp"

int main() {
    poe::undirected_graph graph(3);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 2);
    using mint = poe::modint998244353;
    assert(poe::count_acyclic_edge_states<mint>(graph) == mint{13});
}

