#include "graph/all.hpp"

int main() {
    {
        std::istringstream input("1 2\n2 4\n4 3\n");
        auto* original = std::cin.rdbuf(input.rdbuf());
        poe::directed_graph<> graph(4);
        graph.read(3);
        std::cin.rdbuf(original);
        std::cin.clear();
        assert(graph.edge_count() == 3);
        assert(graph.edge_at(0).from == 0 && graph.edge_at(0).to == 1);
        assert(graph.edge_at(2).from == 3 && graph.edge_at(2).to == 2);
    }

    {
        std::istringstream input("0 1 7\n2 1 -4\n");
        auto* original = std::cin.rdbuf(input.rdbuf());
        poe::undirected_graph<long long> graph(3);
        graph.read(2, 0);
        std::cin.rdbuf(original);
        std::cin.clear();
        assert(graph.edge_count() == 2);
        assert(graph.edge_at(0).cost == 7 && graph.edge_at(1).cost == -4);
        assert(graph.degree(1) == 2);
    }

    poe::directed_graph<> directed(4);
    const int first = directed.add_edge(0, 1);
    directed.add_edge(0, 2);
    directed.add_edge(2, 3);
    assert(first == 0 && directed.size() == 4 && directed.edge_count() == 3);
    assert(directed.outdegree(0) == 2 && directed.indegree(1) == 1);
    assert(directed.edge_at(0).from == 0 && directed.edge_at(0).to == 1);
    const auto reversed = directed.reversed();
    assert(reversed[1][0].to == 0);

    poe::undirected_graph<long long> weighted(5);
    weighted.add_edge(0, 1, 4);
    weighted.add_edge(1, 2, 1);
    weighted.add_edge(0, 2, 3);
    weighted.add_edge(2, 3, 2);
    assert(weighted.degree(2) == 3 && weighted[1][0].cost == 4);

    const auto shortest = poe::dijkstra(weighted, 0);
    assert(shortest[3] == 5);
    assert(shortest.path(3).front() == 0 && shortest.path(3).back() == 3);
    assert(shortest.edge_path(3).size() == 2);

    poe::directed_graph<int> binary(4);
    binary.add_edge(0, 1, 1);
    binary.add_edge(0, 2, 0);
    binary.add_edge(2, 1, 0);
    binary.add_edge(1, 3, 1);
    assert(poe::zero_one_bfs(binary, 0)[3] == 1);
    const auto implicit_binary = poe::zero_one_bfs(4, 0, [&](int vertex, auto relax) {
        for (const auto& edge : binary[vertex]) relax(edge.to, edge.cost);
    });
    assert(implicit_binary[3] == 1 && implicit_binary.path(3).front() == 0);
    assert(poe::bfs(binary, 0)[3] == 2);

    poe::directed_graph<long long> negative(4);
    negative.add_edge(0, 1, 2);
    negative.add_edge(1, 2, -5);
    negative.add_edge(2, 3, 1);
    const auto bellman = poe::bellman_ford(negative, 0);
    assert(bellman[3] == -2 && !bellman.has_negative_cycle());

    const auto topological = poe::topological_sort(directed);
    assert(topological.is_dag() && !topological.order.empty());
    const auto minimum_order = poe::topological_sort_min(directed);
    assert(minimum_order.order == std::vector<int>({0, 1, 2, 3}));

    poe::undirected_graph<> plain(6);
    plain.add_edge(0, 1);
    plain.add_edge(1, 2);
    plain.add_edge(2, 0);
    plain.add_edge(2, 3);
    plain.add_edge(3, 4);
    const auto components = poe::connected_components(plain);
    assert(components.count == 2 && components.same(0, 4) && !components.same(0, 5));
    assert(components.edge_count[components[0]] == 5);
    assert(!components.is_tree(components[0]) && components.is_tree(components[5]));
    assert(!poe::is_bipartite(plain));

    const auto links = poe::lowlink(plain);
    assert(links.articulation_points == std::vector<int>({2, 3}));
    assert(links.bridge_ids == std::vector<int>({3, 4}));
    const auto blocks = poe::two_edge_connected_components(plain);
    assert(blocks.count == 4 && blocks.same(0, 2) && !blocks.same(2, 3));

    const auto forest = poe::minimum_spanning_forest(weighted);
    assert(forest.cost == 6 && forest.component_count == 2 && !forest.connected());

    const auto [scc_count, scc_id] = poe::scc_ids(directed);
    assert(scc_count == 4 && scc_id[0] < scc_id[3]);

    static_assert(poe::graph_type<poe::directed_graph<>>);
    static_assert(poe::directed_graph_type<poe::directed_graph<int>>);
    static_assert(poe::undirected_graph_type<poe::undirected_graph<>>);
    static_assert(poe::weighted_graph_type<poe::undirected_graph<long long>>);
}
