#include "math/bitcountsum.hpp"

int main() {
    std::mt19937_64 random(20260718);
    for (int trial = 0; trial < 100000; ++trial) {
        const long long count = random() % 100;
        const long long first = random() % 10000;
        const long long difference = random() % 1000;
        long long expected = 0;
        for (long long i = 0; i < count; ++i) {
            expected += std::popcount(static_cast<unsigned long long>(first + i * difference));
        }
        assert(poe::sum_popcount_arithmetic_progression(count, first, difference)
               == expected);
    }
}
