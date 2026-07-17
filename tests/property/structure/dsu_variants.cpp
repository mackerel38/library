#include "structure/dsu.hpp"

int main() {
    std::mt19937 random(914713);
    for (int trial = 0; trial < 300; ++trial) {
        const int n = 1 + random() % 8;
        poe::paritydsu uf(n);
        std::vector<std::tuple<int, int, int>> constraints;
        for (int operation = 0; operation < 20; ++operation) {
            constraints.emplace_back(random() % n, random() % n, random() % 2);
            uf.merge(std::get<0>(constraints.back()), std::get<1>(constraints.back()),
                     std::get<2>(constraints.back()));
            int best = n + 1;
            for (int mask = 0; mask < (1 << n); ++mask) {
                bool valid = true;
                for (auto [a, b, difference] : constraints)
                    valid &= (((mask >> a) ^ (mask >> b)) & 1) == difference;
                if (valid) best = std::min(best, std::popcount(static_cast<unsigned>(mask)));
            }
            assert(uf.minimum_color_size() == (best == n + 1 ? -1 : best));
        }
    }
    for (int trial = 0; trial < 300; ++trial) {
        const int n = 1 + random() % 30;
        std::vector<long long> hidden(n);
        for (auto& value : hidden) value = static_cast<int>(random() % 201) - 100;
        poe::weighted_dsu<long long> uf(n);
        for (int operation = 0; operation < 200; ++operation) {
            const int a = random() % n;
            const int b = random() % n;
            assert(uf.merge(a, b, hidden[b] - hidden[a]));
            if (uf.same(a, b)) assert(uf.diff(a, b) == hidden[b] - hidden[a]);
        }
    }

    for (int trial = 0; trial < 200; ++trial) {
        const int n = 1 + random() % 20;
        poe::rollbackdsu uf(n);
        std::vector<std::pair<int, int>> edges;
        std::vector<int> snapshots{uf.snapshot()};
        for (int operation = 0; operation < 300; ++operation) {
            if (edges.empty() || random() % 4 != 0) {
                const int a = random() % n;
                const int b = random() % n;
                uf.merge(a, b);
                edges.emplace_back(a, b);
                snapshots.push_back(uf.snapshot());
            } else {
                const int keep = random() % edges.size();
                edges.resize(keep);
                uf.rollback(snapshots[keep]);
                snapshots.resize(keep + 1);
            }

            std::vector<std::vector<int>> graph(n);
            for (auto [a, b] : edges) {
                graph[a].push_back(b);
                graph[b].push_back(a);
            }
            for (int start = 0; start < n; ++start) {
                std::vector<bool> seen(n);
                std::queue<int> queue;
                seen[start] = true;
                queue.push(start);
                while (!queue.empty()) {
                    int vertex = queue.front();
                    queue.pop();
                    for (int next : graph[vertex]) {
                        if (!seen[next]) seen[next] = true, queue.push(next);
                    }
                }
                for (int vertex = 0; vertex < n; ++vertex) {
                    assert(uf.same(start, vertex) == seen[vertex]);
                }
            }
        }
    }
}
