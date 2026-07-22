#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "flow/matching.hpp"

int main() {
    using edge = poe::weightedbipartiteedge<long long>;
    std::mt19937 random(20260716);
    for (int trial = 0; trial < 500; ++trial) {
        const int left_size = random() % 5;
        const int right_size = random() % 5;
        std::vector<edge> edges;
        for (int left = 0; left < left_size; ++left) {
            for (int right = 0; right < right_size; ++right) {
                if (random() % 2) edges.push_back({left, right, static_cast<int>(random() % 15) - 8});
            }
        }
        long long brute = 0;
        const int maximum_size = std::min(left_size, right_size);
        const long long infinity = (1LL << 60);
        std::vector<long long> minimum(maximum_size + 1, infinity);
        std::vector<long long> maximum(maximum_size + 1, -infinity);
        minimum[0] = maximum[0] = 0;
        for (int mask = 0; mask < (1 << edges.size()); ++mask) {
            int used_left = 0, used_right = 0;
            long long weight = 0;
            int size = 0;
            bool valid = true;
            for (int i = 0; i < static_cast<int>(edges.size()); ++i) {
                if (!(mask >> i & 1)) continue;
                const int left_bit = 1 << edges[i].left;
                const int right_bit = 1 << edges[i].right;
                if ((used_left & left_bit) || (used_right & right_bit)) valid = false;
                used_left |= left_bit;
                used_right |= right_bit;
                weight += edges[i].weight;
                ++size;
            }
            if (valid) {
                brute = std::min(brute, weight);
                minimum[size] = std::min(minimum[size], weight);
                maximum[size] = std::max(maximum[size], weight);
            }
        }
        assert(
            poe::minimum_weight_bipartite_matching(left_size, right_size, edges) == brute
        );
        while (!minimum.empty() && minimum.back() == infinity) minimum.pop_back();
        while (!maximum.empty() && maximum.back() == -infinity) maximum.pop_back();
        assert(poe::minimum_weight_bipartite_matching_values(
            left_size, right_size, edges
        ) == minimum);
        assert(poe::maximum_weight_bipartite_matching_values(
            left_size, right_size, edges
        ) == maximum);
    }
}
