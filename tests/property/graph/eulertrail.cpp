#include "graph/eulertrail.hpp"

template <class Graph>
bool brute(const Graph& graph) {
    std::vector<bool> used(graph.edge_count());
    auto search = [&](auto&& self, int vertex, int count) -> bool {
        if (count == graph.edge_count()) return true;
        for (const auto& edge : graph[vertex]) {
            if (used[edge.id]) continue;
            used[edge.id] = true;
            if (self(self, edge.to, count + 1)) return true;
            used[edge.id] = false;
        }
        return false;
    };
    if (graph.edge_count() == 0) return true;
    for (int start = 0; start < graph.size(); ++start) {
        if (search(search, start, 0)) return true;
    }
    return false;
}

template <class Graph>
void validate(const Graph& graph, const poe::eulertrailresult& result) {
    if (!result) return;
    assert(result.edge_path.size() == static_cast<std::size_t>(graph.edge_count()));
    assert(result.vertex_path.size() == result.edge_path.size() + 1);
    std::vector<bool> used(graph.edge_count());
    for (int index = 0; index < graph.edge_count(); ++index) {
        const int id = result.edge_path[index];
        assert(!used[id]);
        used[id] = true;
        const auto& edge = graph.edge_at(id);
        if constexpr (Graph::is_directed) {
            assert(edge.from == result.vertex_path[index]);
            assert(edge.to == result.vertex_path[index + 1]);
        } else {
            assert((edge.from == result.vertex_path[index] &&
                    edge.to == result.vertex_path[index + 1]) ||
                   (edge.to == result.vertex_path[index] &&
                    edge.from == result.vertex_path[index + 1]));
        }
    }
}

int main() {
    std::mt19937 random(418273);
    for (int trial = 0; trial < 2000; ++trial) {
        const int n = 1 + random() % 5;
        const int m = random() % 8;
        poe::directed_graph<> directed(n);
        poe::undirected_graph<> undirected(n);
        for (int edge = 0; edge < m; ++edge) {
            const int from = random() % n;
            const int to = random() % n;
            directed.add_edge(from, to);
            undirected.add_edge(from, to);
        }
        const auto directed_result = poe::euler_trail(directed);
        const auto undirected_result = poe::euler_trail(undirected);
        assert(static_cast<bool>(directed_result) == brute(directed));
        assert(static_cast<bool>(undirected_result) == brute(undirected));
        validate(directed, directed_result);
        validate(undirected, undirected_result);
    }
}
