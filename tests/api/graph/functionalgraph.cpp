#include "graph/functionalgraph.hpp"

int main() {
    poe::functionalgraph graph({1, 2, 3, 1, 5, 4});
    assert(graph.size() == 6 && graph[0] == 1);
    assert(graph.cycle_count() == 2);
    assert(graph.distance_to_cycle(0) == 1);
    assert(graph.entry(0) == 1);
    assert(graph.cycle_size(0) == 3);
    assert(graph.jump(0, 4) == 1);
    assert(graph.steps(0, 3) == 3);
    assert(graph.steps(3, 2) == 2);
    assert(!graph.steps(2, 0));
    assert(!graph.steps(0, 4));
}
