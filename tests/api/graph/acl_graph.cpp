#include "graph/scc.hpp"
#include "graph/twosat.hpp"

int main() {
    poe::sccgraph graph(6);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 0);
    graph.add_edge(2, 3);
    graph.add_edge(3, 4);
    graph.add_edge(4, 3);
    graph.add_edge(4, 5);
    const auto groups = graph.scc();
    assert(groups == std::vector<std::vector<int>>({{0, 1, 2}, {3, 4}, {5}}));

    poe::twosat sat(3);
    sat.add_clause(0, true, 1, true);
    sat.add_clause(0, false, 2, true);
    sat.add_clause(1, false, 2, false);
    assert(sat.satisfiable());
    const auto answer = sat.answer();
    assert((answer[0] || answer[1]) && (!answer[0] || answer[2]) &&
           (!answer[1] || !answer[2]));

    poe::twosat helpers(5);
    helpers.force(0, true);
    helpers.imply(0, true, 1, false);
    helpers.add_xor(1, false, 2, true);
    helpers.add_equal(2, true, 3, false);
    helpers.at_most_one({{0, true}, {4, true}});
    assert(helpers.satisfiable());
    assert(helpers[0] && !helpers[1] && !helpers[2] && helpers[3] && !helpers[4]);

    poe::twosat impossible(1);
    impossible.add_clause(0, true, 0, true);
    impossible.add_clause(0, false, 0, false);
    assert(!impossible.satisfiable());
}
