#include <cassert>
#include <random>

#include "fps/setfps.hpp"

int main() {
    using mint = poe::modint998244353;
    std::mt19937 random(20260720);
    for (int vertices = 0; vertices <= 10; ++vertices) {
        const int size = 1 << vertices;
        std::vector<mint> value(size);
        for (int mask = 1; mask < size; ++mask) value[mask] = random() % 1000;
        assert(poe::setfps_log<998244353>(poe::setfps_exp<998244353>(value)) == value);
    }
}
