#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "algorithm/circular.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = 1 + random() % 7;
        const int modulus = 1 + random() % 10;
        std::vector<int> values(n);
        for (int& value : values) value = static_cast<int>(random() % 31) - 15;
        int brute = modulus;
        for (int start = 0; start < modulus; ++start) {
            int maximum = 0;
            for (int value : values) {
                value = (value % modulus + modulus) % modulus;
                maximum = std::max(maximum, (value - start + modulus) % modulus);
            }
            brute = std::min(brute, maximum);
        }
        assert(poe::minimum_modular_range(values, modulus) == brute);
    }
}
