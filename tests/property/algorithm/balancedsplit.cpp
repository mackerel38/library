#include <cassert>
#include <queue>
#include <random>
#include "algorithm/balancedsplit.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int n = 1; n <= 8; ++n) {
        for (int test = 0; test < 2000; ++test) {
            std::vector<int> values(n);
            std::priority_queue<long double> pieces;
            for (int& value : values) {
                value = 1 + random() % 100;
                pieces.push(value);
            }
            const int operations = random() % 30;
            for (int step = 0; step < operations; ++step) {
                const long double value = pieces.top() / 2;
                pieces.pop();
                pieces.push(value);
                pieces.push(value);
            }
            std::vector<long double> expected;
            while (!pieces.empty()) {
                expected.push_back(pieces.top());
                pieces.pop();
            }
            for (int rank = 1; rank <= static_cast<int>(expected.size()); ++rank) {
                const long double actual = poe::kth_largest_after_max_halving(
                    values, operations, rank);
                assert(std::abs(actual - expected[rank - 1]) < 1e-12L);
            }
        }
    }
}
