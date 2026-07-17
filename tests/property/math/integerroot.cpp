#include <cassert>
#include <random>

#include "math/math.hpp"

int main() {
    std::mt19937_64 random(428);
    for (int trial = 0; trial < 1000000; ++trial) {
        const unsigned long long value = random();
        const auto root = poe::floor_sqrt(value);
        assert(static_cast<unsigned __int128>(root) * root <= value);
        assert(static_cast<unsigned __int128>(root + 1) * (root + 1) > value);
        assert(poe::is_square(value) ==
               (static_cast<unsigned __int128>(root) * root == value));
    }
    for (unsigned long long root = 0; root <= 1000000; ++root) {
        const unsigned long long square = root * root;
        assert(poe::floor_sqrt(square) == root);
        assert(poe::is_square(square));
        if (square) assert(poe::floor_sqrt(square - 1) == root - 1);
    }
}
