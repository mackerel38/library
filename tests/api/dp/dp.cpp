#include <cassert>
#include <string>
#include <vector>

#include "dp/knapsack.hpp"
#include "dp/optimization.hpp"
#include "dp/sequence.hpp"
#include "dp/subsetsum.hpp"
#include "math/subsettransform.hpp"
#include "tree/cartesiantree.hpp"

int main() {
    {
        const auto possible = poe::subsetsum({1, 3, 3}, 7);
        assert(possible[0] && possible[1] && possible[3] && possible[4]);
        assert(possible[6] && possible[7] && !possible[2]);
        const auto bounded = poe::bounded_subsetsum({2, 5}, {2, 1}, 9);
        assert(bounded[0] && bounded[4] && bounded[7] && bounded[9]);
        assert(!bounded[1] && !bounded[6]);
    }
    {
        assert(poe::knapsack01<long long>({3, 4, 5}, {30, 50, 60}, 8) == 90);
        const auto by_value = poe::knapsack01_by_value({3, 4, 5}, {30, 50, 60});
        assert(by_value[90] == 8);
        assert(poe::knapsack01_value({3, 4, 5}, {30, 50, 60}, 8) == 90);
        assert(poe::knapsack01_mitm<long long>({3, 4, 5}, {30, 50, 60}, 8) == 90);
        assert(poe::knapsack01_auto({3, 4, 5}, {30, 50, 60}, 8) == 90);
        assert(poe::unbounded_knapsack<long long>({3, 4}, {4, 5}, 10) == 13);
        assert(poe::minimum_unbounded_knapsack_cost({3, 4}, {4, 5}, 13) == 10);
        assert(poe::minimum_unbounded_knapsack_cost(
            {2, 3}, {3, 4}, 1'000'000'000'000'000'000LL
        ) == 666'666'666'666'666'667LL);
        assert(poe::bounded_knapsack<long long>({3, 4}, {4, 5}, {2, 1}, 10) == 13);
        assert(poe::knapsack01_auto(
            std::vector<long long>(100, 1'000'000'000'000LL),
            std::vector<long long>(100, 1),
            50'000'000'000'000LL
        ) == 50);
        assert(poe::knapsack01_auto(
            std::vector<long long>(20, 1'000'000'000LL),
            std::vector<long long>(20, 1'000'000'000LL),
            10'000'000'000LL
        ) == 10'000'000'000LL);
    }
    {
        const std::vector<int> values{3, 1, 2, 2, 4};
        const auto increasing = poe::lis(values);
        assert(increasing.size() == 3);
        for (int i = 1; i < increasing.size(); ++i) {
            assert(values[increasing.indices[i - 1]] < values[increasing.indices[i]]);
        }
        assert(poe::lnds(values).size() == 4);
        const std::vector<std::pair<int, int>> intervals{
            {0, 10}, {2, 8}, {3, 7}, {1, 4}, {6, 9}
        };
        const auto nested = poe::longest_nested_intervals(intervals);
        assert(nested.size() == 3);
        for (int i = 1; i < nested.size(); ++i) {
            const auto [outer_left, outer_right] = intervals[nested.indices[i - 1]];
            const auto [inner_left, inner_right] = intervals[nested.indices[i]];
            assert(outer_left < inner_left && inner_right < outer_right);
        }
        const auto common = poe::lcs(std::string("axyb"), std::string("abyxb"));
        assert(common.size() == 3);
        assert(common == "axb" || common == "ayb");
        assert(poe::minimum_unbordered_pattern_increase(
            std::string("ATCABC"), std::string("ABC"), 1
        ) == 1);
        assert(poe::minimum_unbordered_pattern_increase(
            std::string("ABABC"), std::string("ABC"), 1
        ) == -1);
        assert(poe::minimum_unbordered_pattern_increase(
            std::string("ZZZZZ"), std::string("ABC"), 1
        ) == 3);
    }
    {
        assert(poe::optimal_merge_cost(std::vector<long long>{10, 20, 30, 40}) == 190);
        const long long inf = (1LL << 60);
        const auto result = poe::divide_conquer_dp<long long>(
            1, 5, {0, 1, 4, 9, 16},
            [](int, int before, int column) {
                const long long distance = column - before;
                return distance * distance;
            },
            inf
        );
        assert(result == std::vector<long long>({0, 1, 2, 5, 8}));
    }
    {
        std::vector<long long> values{1, 2, 3, 4};
        const auto original = values;
        poe::subset_zeta(values);
        assert(values == std::vector<long long>({1, 3, 4, 10}));
        poe::subset_mobius(values);
        assert(values == original);
        poe::superset_zeta(values);
        assert(values == std::vector<long long>({10, 6, 7, 4}));
        poe::superset_mobius(values);
        assert(values == original);
    }
    {
        const std::vector<int> values{3, 1, 4, 1, 5};
        const auto tree = poe::cartesian_tree(values);
        assert(tree.root == 1);
        assert(tree.parent[0] == 1 && tree.parent[3] == 1);
        assert(tree.left[1] == 0 && tree.right[1] == 3);
        assert(tree.left[3] == 2 && tree.right[3] == 4);
        const auto max_tree = poe::cartesian_tree(values, std::greater{});
        assert(max_tree.root == 4);
        assert(max_tree.left[4] == 2);
    }
}
