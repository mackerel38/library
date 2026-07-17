#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "graph/graph.hpp"
#include "math/modint.hpp"
#include "structure/linecontainer.hpp"
#include "string/subsequence.hpp"
#include "tree/statictoptree.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int topology = 0; topology < 2; ++topology) {
        constexpr int n = 5000;
        poe::undirected_graph<> graph(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            graph.add_edge(topology == 0 ? vertex - 1 : 0, vertex);
        }
        poe::statictoptree tree(graph);
        int maximum_depth = 0;
        for (int node = 0; node < tree.size(); ++node) {
            int depth = 0;
            for (int current = node; tree.parent(current) != -1; current = tree.parent(current)) ++depth;
            maximum_depth = std::max(maximum_depth, depth);
        }
        assert(tree.vertex_count() == n);
        assert(maximum_depth <= 4 * std::bit_width(static_cast<unsigned int>(n)));
    }
    for (int trial = 0; trial < 200; ++trial) {
        const int n = random() % 10;
        std::string text(n, 'a');
        for (char& c : text) c += random() % 3;
        std::set<std::string> set;
        for (int mask = 1; mask < (1 << n); ++mask) {
            std::string value;
            for (int i = 0; i < n; ++i) if (mask >> i & 1) value += text[i];
            set.insert(value);
        }
        const std::vector<std::string> values(set.begin(), set.end());
        for (std::size_t k = 1; k <= values.size(); ++k) {
            assert(poe::kth_distinct_subsequence(text, k, "abc") == values[k - 1]);
        }
        assert(!poe::kth_distinct_subsequence(text, values.size() + 1, "abc"));
    }
    for (int trial = 0; trial < 100; ++trial) {
        const int n = 1 + random() % 60;
        poe::undirected_graph<> graph(n);
        std::vector<int> parent(n, -1);
        for (int vertex = 1; vertex < n; ++vertex) {
            parent[vertex] = random() % vertex;
            graph.add_edge(parent[vertex], vertex);
        }
        using mint = poe::modint998244353;
        std::vector<mint> value(n);
        for (auto& x : value) x = random() % 100;
        struct path { mint a, b; };
        using point = mint;
        poe::statictoptree tree(graph);
        auto dp = poe::make_statictoptreedp<path, point>(
            tree,
            [&](int vertex) { return path{1, value[vertex]}; },
            [](path p, path c) { return path{p.a * c.a, p.a * c.b + p.b}; },
            [](point l, point r) { return l * r; },
            [](path child) { return child.b; },
            [&](point children, int vertex) { return path{children, value[vertex]}; }
        );
        auto brute = [&] {
            std::vector<mint> result(n);
            for (int vertex = n - 1; vertex >= 0; --vertex) {
                mint product = 1;
                bool has_child = false;
                for (int child = vertex + 1; child < n; ++child) {
                    if (parent[child] == vertex) {
                        has_child = true;
                        product *= result[child];
                    }
                }
                result[vertex] = value[vertex] + (has_child ? product : mint{});
            }
            return result[0];
        };
        assert(dp.get().b == brute());
        for (int query = 0; query < 30; ++query) {
            const int vertex = random() % n;
            value[vertex] = random() % 100;
            dp.update(vertex);
            assert(dp.get().b == brute());
        }
    }

    for (int trial = 0; trial < 100; ++trial) {
        std::vector<long long> coordinates(30);
        for (int i = 0; i < 30; ++i) coordinates[i] = i - 15;
        poe::minlinecontainer<long long> tree(coordinates);
        std::vector<poe::line<long long>> lines;
        for (int query = 0; query < 50; ++query) {
            poe::line<long long> current{
                static_cast<int>(random() % 41) - 20,
                static_cast<int>(random() % 101) - 50
            };
            lines.push_back(current);
            tree.add_line(current);
            for (long long x : coordinates) {
                long long answer = 1LL << 60;
                for (const auto line : lines) answer = std::min(answer, line.get(x));
                assert(tree.get(x) == answer);
            }
        }
    }
}
