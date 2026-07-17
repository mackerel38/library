#include "all.hpp"

int main() {
    poe::dsu uf(2);
    uf.merge(0, 1);
    assert(uf.same(0, 1));

    poe::modint998244353 value = 2;
    assert(value.pow(10) == 1024);

    assert(poe::z_algorithm(std::string("aaa")) == std::vector<int>({3, 2, 1}));

    poe::maxflow<int> graph(2);
    graph.add_edge(0, 1, 1);
    assert(graph.flow(0, 1) == 1);
}
