#include "tree/dsuontree.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int n = 1; n <= 80; ++n) {
        for (int trial = 0; trial < 80; ++trial) {
            std::vector<int> parent(n, -1), color(n);
            for (int vertex = 1; vertex < n; ++vertex) parent[vertex] = random() % vertex;
            for (int& value : color) value = random() % 9;
            poe::dsuontree tree(parent);
            std::vector<std::array<int, 9>> actual(n);
            std::array<int, 9> count{};
            tree.run(
                [&](int vertex) { ++count[color[vertex]]; },
                [&](int vertex) { --count[color[vertex]]; },
                [&](int vertex) { actual[vertex] = count; });
            for (int vertex = 0; vertex < n; ++vertex) {
                std::array<int, 9> expected{};
                for (int i = tree.index(vertex); i < tree.out(vertex); ++i) {
                    ++expected[color[tree[i]]];
                }
                assert(actual[vertex] == expected);
            }
            assert((count == std::array<int, 9>{}));
        }
    }
}
