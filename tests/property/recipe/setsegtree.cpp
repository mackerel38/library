#include <cassert>
#include <random>

#include "recipe/setsegtree.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937_64 random(20260716);
    for (int n = 1; n <= 30; ++n) {
        for (int repetition = 0; repetition < 50; ++repetition) {
            vector<uint64_t> masks(n);
            for (auto& mask : masks) mask = random() & ((uint64_t{1} << 12) - 1);
            setsegtree tree(masks);
            for (int query = 0; query < 1000; ++query) {
                int left = random() % n;
                int right = random() % n;
                if (left > right) swap(left, right);
                ++right;
                const int type = random() % 5;
                const uint64_t mask = random() & ((uint64_t{1} << 12) - 1);
                if (type == 0) {
                    tree.add_mask(left, right, mask);
                    for (int i = left; i < right; ++i) masks[i] |= mask;
                } else if (type == 1) {
                    tree.erase_mask(left, right, mask);
                    for (int i = left; i < right; ++i) masks[i] &= ~mask;
                } else if (type == 2) {
                    const uint64_t keep = random() & ((uint64_t{1} << 12) - 1);
                    tree.apply(left, right, keep, mask);
                    for (int i = left; i < right; ++i) masks[i] = (masks[i] & keep) | mask;
                } else if (type == 3) {
                    int maximum = -1;
                    int count = 0;
                    for (int i = left; i < right; ++i) {
                        const int set_size = popcount(masks[i]);
                        if (set_size > maximum) maximum = set_size, count = 0;
                        if (set_size == maximum) ++count;
                    }
                    const auto result = tree.max(left, right);
                    assert(result.size == maximum && result.count == count);
                } else {
                    const int position = random() % n;
                    assert(tree[position] == masks[position]);
                }
            }
            for (int i = 0; i < n; ++i) assert(tree[i] == masks[i]);
        }
    }
}
