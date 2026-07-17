#include <cassert>
#include <random>
#include "structure/intervalset.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int trial = 0; trial < 200; ++trial) {
        poe::intervalset<int> intervals;
        std::vector<bool> used(40);
        for (int query = 0; query < 300; ++query) {
            int left = random() % 41, right = random() % 41;
            if (left > right) std::swap(left, right);
            const bool add = random() & 1;
            int changed = 0;
            for (int i = left; i < right; ++i) {
                if (used[i] != add) ++changed;
                used[i] = add;
            }
            assert((add ? intervals.add(left, right) : intervals.erase(left, right)) == changed);
            assert(intervals.covered() == std::count(used.begin(), used.end(), true));
            for (int i = 0; i < 40; ++i) assert(intervals.contains(i) == used[i]);
        }
    }
}
