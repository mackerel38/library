#include <cassert>
#include <cstdint>
#include <vector>
#include "dp/subset.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = poe::modint998244353;
    assert(poe::count_distinct_deletion_orders<mint>(std::string("aab")).val() == 3);
    assert(poe::count_distinct_deletion_orders<mint>(std::string("abc")).val() == 6);

    constexpr int inf = 1'000'000;
    vector<int> cost(1 << 4, inf);
    cost[0b0011] = 5;
    cost[0b1100] = 2;
    cost[0b1111] = 10;
    for (int vertex = 0; vertex < 4; ++vertex) cost[1 << vertex] = 4;

    auto result = subset_partition_min(4, cost, inf);
    assert(result.cost == 7);
    uint64_t used = 0;
    for (uint64_t group : result.groups) {
        assert((used & group) == 0);
        used |= group;
    }
    assert(used == 0b1111);

    vector<int> impossible(1 << 2, inf);
    auto none = subset_partition_min(2, impossible, inf);
    assert(none.cost == inf && none.groups.empty());

    auto zero = max_zero_sum_partition(vector<int>{2, -2, 3, -1, -2, 0});
    assert(zero.groups.size() == 3);
    uint64_t zero_used = 0;
    for (uint64_t group : zero.groups) {
        assert((zero_used & group) == 0);
        zero_used |= group;
        int sum = 0;
        for (int index = 0; index < 6; ++index) if (group >> index & 1) sum += vector<int>{2, -2, 3, -1, -2, 0}[index];
        assert(sum == 0);
    }
    assert(zero_used == 0b111111);
    assert(max_zero_sum_partition(vector<int>{1, 2}).groups.empty());
}
