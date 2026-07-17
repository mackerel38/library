#include "graph/scc.hpp"
#include "graph/twosat.hpp"

struct clause {
    int i;
    bool f;
    int j;
    bool g;
};

int main() {
    std::mt19937 random(246810);
    for (int n = 0; n <= 8; ++n) {
        for (int iteration = 0; iteration < 1000; ++iteration) {
            const int count = n == 0 ? 0 : random() % 20;
            std::vector<clause> clauses;
            poe::twosat sat(n);
            for (int k = 0; k < count; ++k) {
                clause current{static_cast<int>(random() % n), bool(random() & 1),
                               static_cast<int>(random() % n), bool(random() & 1)};
                clauses.push_back(current);
                sat.add_clause(current.i, current.f, current.j, current.g);
            }
            int valid_mask = -1;
            for (int mask = 0; mask < (1 << n); ++mask) {
                bool ok = true;
                for (const auto& current : clauses) {
                    ok &= (((mask >> current.i) & 1) == current.f) ||
                          (((mask >> current.j) & 1) == current.g);
                }
                if (ok) {
                    valid_mask = mask;
                    break;
                }
            }
            assert(sat.satisfiable() == (valid_mask != -1));
            if (valid_mask != -1) {
                const auto answer = sat.answer();
                for (const auto& current : clauses) {
                    assert(answer[current.i] == current.f || answer[current.j] == current.g);
                }
            }
        }
    }

    for (int n = 1; n <= 70; ++n) {
        poe::sccgraph graph(n);
        std::vector<std::pair<int, int>> edges;
        for (int k = 0; k < n * 4; ++k) {
            const int from = random() % n;
            const int to = random() % n;
            graph.add_edge(from, to);
            edges.emplace_back(from, to);
        }
        const auto [count, ids] = graph.scc_ids();
        assert(1 <= count && count <= n);
        for (const auto& [from, to] : edges) {
            if (ids[from] != ids[to]) {
                assert(ids[from] < ids[to]);
            }
        }
    }

    constexpr int long_path_size = 200000;
    poe::sccgraph long_path(long_path_size);
    for (int vertex = 0; vertex + 1 < long_path_size; ++vertex)
        long_path.add_edge(vertex, vertex + 1);
    const auto [long_path_count, long_path_ids] = long_path.scc_ids();
    assert(long_path_count == long_path_size);
    for (int vertex = 0; vertex < long_path_size; ++vertex)
        assert(long_path_ids[vertex] == vertex);
}
