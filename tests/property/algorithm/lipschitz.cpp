#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include "algorithm/lipschitz.hpp"

using namespace poe;

int main() {
    for (int size = 0; size <= 8; ++size) {
        for (int mask = 0; mask < (1 << size); ++mask) {
            std::vector<int> upper(size);
            for (int i = 0; i < size; ++i) upper[i] = 2 + ((mask >> i) & 1) * 5;
            for (int slope = 0; slope <= 3; ++slope) {
                const auto actual = lipschitz_minorant(upper, slope);
                for (int i = 0; i < size; ++i) {
                    int expected = upper[i];
                    for (int j = 0; j < size; ++j) {
                        expected = std::min(expected, upper[j] + slope * std::abs(i - j));
                    }
                    assert(actual[i] == expected);
                    if (i) assert(std::abs(actual[i] - actual[i - 1]) <= slope);
                }
            }
        }
    }
}
