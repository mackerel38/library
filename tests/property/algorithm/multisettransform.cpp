#include "algorithm/multisettransform.hpp"

using namespace std;
using namespace poe;

int distance(unsigned source, unsigned target) {
    int target_moves = 0;
    while (true) {
        unsigned current = source;
        int source_moves = 0;
        while (true) {
            if (current == target) return source_moves + target_moves;
            if (current == 0) break;
            current /= 2;
            ++source_moves;
        }
        if (target == 0 || target % 2 == 1) return 1000000;
        target /= 2;
        ++target_moves;
    }
}

int main() {
    mt19937 random(2541);
    for (int iteration = 0; iteration < 5000; ++iteration) {
        const int n = 1 + random() % 7;
        vector<unsigned> source(n), target(n);
        for (auto& value : source) value = random() % 32;
        for (auto& value : target) value = random() % 32;

        vector<int> permutation(n);
        iota(permutation.begin(), permutation.end(), 0);
        int expected = 1000000;
        do {
            int candidate = 0;
            for (int index = 0; index < n; ++index) {
                candidate += distance(source[index], target[permutation[index]]);
                if (candidate >= expected) break;
            }
            expected = min(expected, candidate);
        } while (next_permutation(permutation.begin(), permutation.end()));

        const auto actual = minimum_double_halve_multiset_operations(source, target);
        assert(static_cast<bool>(actual) == (expected < 1000000));
        if (actual) assert(*actual == expected);
    }
}
