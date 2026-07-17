#include <cassert>
#include <random>
#include <vector>
#include "dp/linearinequality.hpp"
static long long brute(const std::vector<int>& weights, int at, int left) {
    if (at == static_cast<int>(weights.size())) return 1;
    long long answer = 0;
    for (int used = 0; used <= left; used += weights[at]) answer += brute(weights, at + 1, left - used);
    return answer;
}
int main() {
    std::mt19937 random(436);
    for (int n = 0; n <= 6; ++n) for (int trial = 0; trial < 300; ++trial) {
        std::vector<int> weights(n); for (int& w : weights) w = random() % 7 + 1;
        const int limit = random() % 40;
        assert(poe::count_nonnegative_weighted_sum_at_most(weights, limit).val() == brute(weights, 0, limit));
    }
}
