#include <cassert>
#include <vector>

#include "dp/aliens.hpp"
#include "dp/profile.hpp"
#include "graph/graph.hpp"
#include "math/combination.hpp"
#include "math/divisortransform.hpp"
#include "math/interpolation.hpp"
#include "math/linearecurrence.hpp"
#include "math/matrix.hpp"
#include "math/modint.hpp"
#include "math/tropicalconvolution.hpp"
#include "structure/doubling.hpp"
#include "structure/linecontainer.hpp"
#include "string/subsequence.hpp"
#include "tree/statictoptree.hpp"

int main() {
    poe::pascalcombination pascal(10, 1000);
    assert(pascal.choose(5, 2) == 10);
    assert(pascal.multinomial(std::vector<int>{2, 2}) == 6);

    using mint = poe::modint998244353;
    {
        assert(*poe::kth_distinct_subsequence("aba", 1) == "a");
        assert(*poe::kth_distinct_subsequence("aba", 5) == "b");
        assert(!poe::kth_distinct_subsequence("aba", 7));
    }
    {
        poe::matrix<mint> fibonacci({{1, 1}, {1, 0}});
        assert(fibonacci.pow(10)[0][1].val() == 55);
        assert(poe::linear_recurrence<mint>({0, 1}, {1, 1}, 10).val() == 55);
        poe::combination<mint> comb(20);
        assert(comb.choose(10, 3).val() == 120);
        assert(comb.choose_large(1'000'000'000LL, 2).val() ==
               (mint{1'000'000'000LL} * mint{999'999'999LL} / mint{2}).val());
        std::vector<mint> sampled;
        for (int x = 0; x < 5; ++x) sampled.push_back(mint{x * x + 2 * x + 3});
        assert(poe::lagrange_consecutive(sampled, 100).val() == 10203);
    }
    {
        std::vector<long long> values(13);
        for (int i = 1; i <= 12; ++i) values[i] = i;
        const auto original = values;
        poe::divisor_zeta(values);
        assert(values[12] == 1 + 2 + 3 + 4 + 6 + 12);
        poe::divisor_mobius(values);
        assert(values == original);
        poe::multiple_zeta(values);
        assert(values[3] == 3 + 6 + 9 + 12);
        poe::multiple_mobius(values);
        assert(values == original);
    }
    {
        poe::minlinecontainer<long long> lines({-2, -1, 0, 1, 2});
        lines.add_line({2, 3});
        lines.add_line({-1, 1});
        assert(lines.get(-2) == -1);
        assert(lines.get(2) == -1);
        poe::maxlinecontainer<long long> maximum({0, 1, 2});
        maximum.add_segment(1, 3, {3, -1});
        assert(maximum.get(2) == 5);
    }
    {
        poe::doubling table({1, 2, 3, 4, -1}, 10);
        assert(table.jump(0, 3) == 3);
        assert(table.jump(2, 3) == -1);
        const auto [steps, vertex] = table.max_jump(0, [](int v) { return v <= 3; });
        assert(steps == 3 && vertex == 3);
    }
    {
        assert(poe::normalize_profile({5, -1, 5, 9}) == std::vector<int>({0, -1, 0, 1}));
        assert(poe::normalize_profile({5, 9, 5}, 1) == std::vector<int>({1, 0, 1}));
        assert(poe::normalize_profile({5, -1, 9}, -1, true) == std::vector<int>({1, -1, 2}));
        const auto result = poe::aliens_min<long long>(-20, 20, 3, [](long long penalty) {
            std::pair<long long, int> best{1LL << 60, 0};
            for (int count = 0; count <= 5; ++count) {
                const long long base = 1LL * (5 - count) * (5 - count);
                best = std::min(best, std::pair{base + penalty * count, count});
            }
            return best;
        });
        assert(result.value == 4);
    }
    {
        const std::vector<long long> concave{0, 5, 8, 9};
        const std::vector<long long> other{1, 5, 7};
        assert(poe::maxplus_convolution_concave(concave, other) == poe::maxplus_convolution(concave, other));
        const std::vector<long long> convex{0, 1, 4, 9};
        assert(poe::minplus_convolution_convex(convex, convex) == poe::minplus_convolution(convex, convex));
    }
    {
        poe::undirected_graph<> graph(5);
        graph.add_edge(0, 1);
        graph.add_edge(0, 2);
        graph.add_edge(1, 3);
        graph.add_edge(1, 4);
        std::vector<mint> value{1, 2, 3, 4, 5};
        struct path { mint a, b; };
        using point = mint;
        poe::statictoptree tree(graph);
        auto dp = poe::make_statictoptreedp<path, point>(
            tree,
            [&](int vertex) { return path{1, value[vertex]}; },
            [](path parent, path child) { return path{parent.a * child.a, parent.a * child.b + parent.b}; },
            [](point left, point right) { return left * right; },
            [](path child) { return child.b; },
            [&](point children, int vertex) { return path{children, value[vertex]}; }
        );
        assert(dp.get().b.val() == 67);
        value[3] = 10;
        dp.update(3);
        assert(dp.get().b.val() == 157);
    }
}
