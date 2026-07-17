#include <algorithm>
#include <cassert>
#include <cstdint>
#include <random>
#include <vector>
#include "dp/subset.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    {
        using mint = poe::modint998244353;
        std::map<std::string, mint> memo;
        const auto brute = [&](auto&& self, const std::string& value) -> mint {
            if (value.empty()) return 1;
            if (memo.contains(value)) return memo[value];
            std::set<std::string> next;
            for (int i = 0; i < static_cast<int>(value.size()); ++i) {
                next.insert(value.substr(0, i) + value.substr(i + 1));
            }
            mint result = 0;
            for (const auto& candidate : next) result += self(self, candidate);
            return memo[value] = result;
        };
        std::mt19937 random(20260716);
        for (int n = 0; n <= 9; ++n) {
            for (int test = 0; test < 300; ++test) {
                std::string value(n, 'a');
                for (char& character : value) character += random() % 3;
                assert(poe::count_distinct_deletion_orders<mint>(value) == brute(brute, value));
            }
        }
    }

    mt19937 random(20260715);
    constexpr int inf = 1'000'000;
    for (int n = 0; n <= 10; ++n) {
        const int count = 1 << n;
        for (int repetition = 0; repetition < 40; ++repetition) {
            vector<int> cost(count, inf);
            for (int mask = 1; mask < count; ++mask) {
                if (uniform_int_distribution<int>(0, 4)(random) != 0) {
                    cost[mask] = uniform_int_distribution<int>(0, 30)(random);
                }
            }
            vector<int> brute(count, inf);
            brute[0] = 0;
            for (int mask = 1; mask < count; ++mask) {
                for (int group = mask; group != 0; group = (group - 1) & mask) {
                    if (cost[group] == inf || brute[mask ^ group] == inf) continue;
                    brute[mask] = min(brute[mask], brute[mask ^ group] + cost[group]);
                }
            }

            auto result = subset_partition_min(n, cost, inf);
            assert(result.cost == brute[count - 1]);
            if (result.cost == inf) {
                assert(result.groups.empty());
                continue;
            }
            uint64_t used = 0;
            int reconstructed_cost = 0;
            for (uint64_t group : result.groups) {
                assert(group != 0 && (used & group) == 0);
                used |= group;
                reconstructed_cost += cost[group];
            }
            assert(used == static_cast<uint64_t>(count - 1));
            assert(reconstructed_cost == result.cost);
        }
    }


    for (int n = 0; n <= 10; ++n) {
        const int count = 1 << n;
        for (int repetition = 0; repetition < 100; ++repetition) {
            vector<int> values(n);
            for (int& value : values) value = uniform_int_distribution<int>(-4, 4)(random);
            vector<int> sums(count);
            for (int mask = 1; mask < count; ++mask) {
                const int bit = countr_zero(static_cast<unsigned>(mask));
                sums[mask] = sums[mask ^ (1 << bit)] + values[bit];
            }
            auto result = max_zero_sum_partition(values);
            if (sums.back() != 0) {
                assert(result.groups.empty());
                continue;
            }
            vector<int> brute(count, -1000);
            brute[0] = 0;
            for (int mask = 1; mask < count; ++mask) {
                const int anchor = mask & -mask;
                for (int group = mask; group; group = (group - 1) & mask) {
                    if ((group & anchor) && sums[group] == 0) {
                        brute[mask] = max(brute[mask], brute[mask ^ group] + 1);
                    }
                }
            }
            assert(static_cast<int>(result.groups.size()) == brute.back());
            uint64_t used = 0;
            for (const uint64_t group : result.groups) {
                assert(group != 0 && (used & group) == 0 && sums[group] == 0);
                used |= group;
            }
            assert(used == static_cast<uint64_t>(count - 1));
        }
    }
}
