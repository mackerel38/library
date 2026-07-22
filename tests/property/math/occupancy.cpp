#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "math/occupancy.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    for (int boxes = 0; boxes <= 4; ++boxes) {
        for (int types = 0; types <= 4; ++types) {
            int capacity_cases = 1;
            for (int type = 0; type < types; ++type) capacity_cases *= boxes + 1;
            for (int encoded = 0; encoded < capacity_cases; ++encoded) {
                int value = encoded;
                vector<int> capacities(types);
                for (int& capacity : capacities) {
                    capacity = value % (boxes + 1);
                    value /= boxes + 1;
                }
                long long expected = 0;
                vector<int> used(types);
                auto brute = [&](auto&& self, int box) -> void {
                    if (box == boxes) {
                        ++expected;
                        return;
                    }
                    for (int subset = 1; subset < (1 << types); ++subset) {
                        bool valid = true;
                        for (int type = 0; type < types; ++type) {
                            if (subset >> type & 1) valid &= ++used[type] <= capacities[type];
                        }
                        if (valid) self(self, box + 1);
                        for (int type = 0; type < types; ++type) {
                            if (subset >> type & 1) --used[type];
                        }
                    }
                };
                brute(brute, 0);
                assert(count_nonempty_subset_occupancies<mint>(boxes, capacities) == expected);
            }
        }
    }
}
