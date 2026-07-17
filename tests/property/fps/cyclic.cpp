#include <cassert>
#include <random>

#include "fps/cyclic.hpp"

int main() {
    using mint = poe::modint998244353;
    std::mt19937 random(20260719);
    for (int test = 0; test < 100; ++test) {
        const int size = 1 + random() % 60;
        const int exponent = random() % 15;
        std::vector<mint> values(size), brute(size), next(size);
        for (auto& value : values) value = random() % 100;
        brute[0] = 1;
        for (int count = 0; count < exponent; ++count) {
            std::fill(next.begin(), next.end(), mint{});
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) next[(i + j) % size] += brute[i] * values[j];
            }
            brute.swap(next);
        }
        assert(poe::cyclic_power<998244353>(values, exponent) == brute);
        std::vector<mint> other(size), product(size);
        for (auto& value : other) value = random() % 100;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                product[(i + j) % size] += values[i] * other[j];
            }
        }
        assert(poe::cyclic_convolution<998244353>(values, other) == product);
    }
}
