#include "flow/dinic.hpp"
#include "flow/flowgraph.hpp"
#include "flow/lowerbound.hpp"
#include "flow/maxflow.hpp"
#include "flow/mincost.hpp"
#include "flow/mincostflow.hpp"
#include "flow/submodular.hpp"

int main() {
    poe::flowgraph<long long> residual(4);
    residual.add_edge(0, 1, 2);
    residual.add_edge(1, 3, 2);
    assert(residual[0].size() == 1);
    assert(poe::dinic(residual, 0, 3) == 2);

    poe::costflowgraph<int, long long> cost_residual(2);
    cost_residual.add_edge(0, 1, 3, 4);
    assert(poe::mincost(cost_residual, 0, 1) == std::pair(3, 12LL));

    poe::maxflow<long long> maximum(4);
    const int first = maximum.add_edge(0, 1, 2);
    maximum.add_edge(0, 2, 1);
    maximum.add_edge(1, 2, 1);
    maximum.add_edge(1, 3, 1);
    maximum.add_edge(2, 3, 2);
    assert(maximum.flow(0, 3) == 3);
    assert(maximum.get_edge(first).flow == 2);
    const auto cut = maximum.min_cut(0);
    assert(cut[0] && !cut[3]);

    poe::maxflow<int> changed(2);
    const int edge = changed.add_edge(0, 1, 5);
    changed.change_edge(edge, 7, 3);
    assert(changed.get_edge(edge).cap == 7);
    assert(changed.get_edge(edge).flow == 3);

    poe::mincostflow<long long, long long> minimum(4);
    minimum.add_edge(0, 1, 2, 1);
    minimum.add_edge(0, 2, 1, 3);
    minimum.add_edge(1, 2, 1, 1);
    minimum.add_edge(1, 3, 1, 4);
    minimum.add_edge(2, 3, 2, 1);
    assert(minimum.flow(0, 3) == std::pair(3LL, 12LL));

    poe::mincostflow<int, int> slope_graph(2);
    slope_graph.add_edge(0, 1, 2, 3);
    slope_graph.add_edge(0, 1, 1, 5);
    assert((slope_graph.slope(0, 1) ==
            std::vector<std::pair<int, int>>({{0, 0}, {2, 6}, {3, 11}})));

    poe::lowerboundflow<long long> bounded(3);
    bounded.add_edge(0, 1, 2, 5);
    bounded.add_edge(1, 2, 1, 4);
    bounded.add_edge(0, 2, 0, 2);
    const auto bounded_max = bounded.maxflow(0, 2);
    const auto bounded_min = bounded.minflow(0, 2);
    assert(bounded_max && bounded_max->flow == 6);
    assert(bounded_min && bounded_min->flow == 2);

    poe::lowerboundflow<int> circulation(2);
    circulation.add_edge(0, 1, 2, 3);
    circulation.add_edge(1, 0, 1, 4);
    const auto circulation_flow = circulation.circulation();
    assert(circulation_flow && (*circulation_flow)[0] == (*circulation_flow)[1]);

    poe::lowerboundflow<int> impossible(2);
    impossible.add_edge(0, 1, 1, 1);
    assert(!impossible.feasible());

    poe::submodular<long long> energy(2);
    energy.add_unary(0, 3, 0);
    energy.add_unary(1, 0, 2);
    energy.add_neq_cost(0, 1, 4);
    const auto minimized = energy.minimize();
    assert(minimized.cost == 2);
    assert(minimized.value[0] && minimized.value[1]);
}
