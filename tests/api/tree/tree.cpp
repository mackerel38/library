#include "tree/all.hpp"

struct rerootvalue {
    long long count = 0;
    long long sum = 0;
};

int main() {
    poe::undirected_graph<long long> graph(7);
    graph.add_edge(0, 1, 3);
    graph.add_edge(0, 2, 2);
    graph.add_edge(1, 3, 4);
    graph.add_edge(1, 4, 1);
    graph.add_edge(2, 5, 5);
    graph.add_edge(5, 6, 2);

    poe::rootedtree rooted(graph, 0);
    assert(rooted.parent(4) == 1 && rooted.depth(6) == 3);
    assert(rooted.distance(6) == 9 && rooted.subtree_size(2) == 3);
    assert(rooted.is_ancestor(2, 6));

    poe::lcatree ancestors(graph, 0);
    assert(ancestors.lca(3, 4) == 1);
    assert(ancestors.lca(3, 6) == 0);
    assert(ancestors.distance(3, 6) == 16);
    assert(ancestors.edge_distance(3, 6) == 5);
    assert(ancestors.kth_ancestor(6, 2) == 2);
    assert(ancestors.jump(3, 6, 3) == 2);
    assert(ancestors.jump(3, 6, 6) == -1);
    assert(ancestors.on_path(0, 3, 6));
    assert(!ancestors.on_path(4, 3, 6));
    assert(ancestors.side_size(1, 0) == 3);
    assert(ancestors.side_size(0, 1) == 4);

    poe::hld decomposition(graph, 0);
    std::vector<int> vertex_path;
    for (const auto& segment : decomposition.path(3, 6)) {
        if (segment.reversed) {
            for (int index = segment.right; index-- > segment.left;) {
                vertex_path.push_back(decomposition.vertex(index));
            }
        } else {
            for (int index = segment.left; index < segment.right; ++index) {
                vertex_path.push_back(decomposition.vertex(index));
            }
        }
    }
    assert(vertex_path == std::vector<int>({3, 1, 0, 2, 5, 6}));
    const auto [left, right] = decomposition.subtree(2);
    assert(right - left == 3);

    const auto diameter = poe::tree_diameter(graph);
    assert(diameter.distance == 16);
    assert(diameter.path.size() == diameter.edge_ids.size() + 1);

    poe::undirected_graph<> plain(4);
    plain.add_edge(0, 1);
    plain.add_edge(1, 2);
    plain.add_edge(1, 3);
    const auto rerooted = poe::rerooting(
        plain, rerootvalue{},
        [](rerootvalue left, rerootvalue right) {
            return rerootvalue{left.count + right.count, left.sum + right.sum};
        },
        [](rerootvalue value, int) {
            ++value.count;
            return value;
        },
        [](rerootvalue value, const auto&) {
            value.sum += value.count;
            return value;
        });
    assert(rerooted[0].sum == 5);
    assert(rerooted[1].sum == 3);
    assert(rerooted[2].sum == 5);
    assert(rerooted[3].sum == 5);
}
