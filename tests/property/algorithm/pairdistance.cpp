#include <cassert>
#include <random>
#include <vector>
#include "algorithm/pairdistance.hpp"
int main() {
    std::mt19937 random(437);
    for (int n = 0; n <= 20; ++n) for (int m = 0; m <= 20; ++m) {
        std::vector<long long> a(n), b(m);
        for (auto& x : a) x = int(random() % 31) - 15;
        for (auto& x : b) x = int(random() % 31) - 15;
        long long expected = 0;
        for (auto x : a) for (auto y : b) expected += std::abs(x - y);
        assert(poe::sum_cross_absolute(a, b) == expected);
    }
}
