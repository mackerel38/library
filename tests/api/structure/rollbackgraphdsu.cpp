#include "structure/dsu.hpp"

using namespace std;
using namespace poe;

int main() {
    rollbackgraphdsu uf(4, [](int vertices, int edges) {
        return min(vertices, edges);
    });
    assert(uf.value() == 0);
    const int initial = uf.snapshot();
    assert(uf.add_edge(0, 1));
    assert(uf.value() == 1 && uf.size(0) == 2 && uf.edges(0) == 1);
    assert(!uf.add_edge(0, 1));
    assert(uf.value() == 2 && uf.edges(1) == 2);
    const int cycle = uf.snapshot();
    assert(uf.add_edge(1, 2));
    assert(uf.value() == 3 && uf.size(2) == 3 && uf.edges(2) == 3);
    uf.rollback(cycle);
    assert(uf.value() == 2 && !uf.same(0, 2));
    uf.rollback(initial);
    assert(uf.value() == 0 && uf.count() == 4);
}
