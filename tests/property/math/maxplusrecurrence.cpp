#include <algorithm>
#include <cassert>
#include <limits>
#include <random>
#include <vector>

#include "math/linearecurrence.hpp"

int main() {
    std::mt19937 random(415);
    const long long negative_infinity = std::numeric_limits<long long>::lowest();
    for (int trial = 0; trial < 250; ++trial) {
        const int order = 1 + random() % 8;
        const int length = order + random() % 80;
        std::vector<long long> initial(order), coefficient(order);
        for (auto& value : initial) value = static_cast<int>(random() % 41) - 20;
        for (auto& value : coefficient) {
            value = random() % 5 == 0
                ? negative_infinity : static_cast<int>(random() % 41) - 20;
        }
        std::vector<long long> brute = initial;
        for (int index = order; index < length; ++index) {
            long long value = negative_infinity;
            for (int step = 1; step <= order; ++step) {
                if (coefficient[step - 1] == negative_infinity ||
                    brute[index - step] == negative_infinity) continue;
                value = std::max(
                    value, brute[index - step] + coefficient[step - 1]);
            }
            brute.push_back(value);
        }
        for (int index = 0; index < length; ++index) {
            assert(poe::maxplus_recurrence(initial, coefficient, index) == brute[index]);
        }
    }
}
