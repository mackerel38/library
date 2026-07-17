#include <cassert>
#include <random>
#include <set>
#include <vector>

#include "structure/excludedset.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int trial = 0; trial < 1000; ++trial) {
        std::set<long long> forbidden;
        const int n = random() % 25;
        while (static_cast<int>(forbidden.size()) < n) forbidden.insert(random() % 60);
        poe::excludedset<long long> data(
            std::vector<long long>(forbidden.begin(), forbidden.end()));
        for (int query = 0; query < 30; ++query) {
            const long long lower = random() % 70;
            const long long rank = 1 + random() % 30;
            long long value = lower, found = 0;
            while (true) {
                if (!forbidden.contains(value) && ++found == rank) break;
                ++value;
            }
            assert(data.kth_allowed(lower, rank) == value);
        }
    }
}
