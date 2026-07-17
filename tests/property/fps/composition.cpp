#include <cassert>
#include <random>

#include "fps/composition.hpp"

int main() {
    using series = poe::fps998244353;
    std::mt19937 random(20260721);
    for (int size = 1; size <= 40; ++size) {
        series outer(size), inner(size), brute(size), power{1};
        for (auto& value : outer) value = random() % 1000;
        for (int i = 1; i < size; ++i) inner[i] = random() % 1000;
        for (int i = 0; i < size; ++i) {
            for (int degree = 0; degree < power.size_int(); ++degree) brute[degree] += outer[i] * power[degree];
            power = (power * inner).prefix(size);
        }
        assert(poe::compose<998244353>(outer, inner, size) == brute);
    }
    for (const int size : {64, 95, 128}) {
        series outer(size), inner(size), brute(size), power{1};
        for (auto& value : outer) value = random() % 1000;
        for (int i = 1; i < size; ++i) inner[i] = random() % 1000;
        for (int i = 0; i < size; ++i) {
            for (int degree = 0; degree < power.size_int(); ++degree) brute[degree] += outer[i] * power[degree];
            power = (power * inner).prefix(size);
        }
        assert(poe::compose<998244353>(outer, inner, size) == brute);
    }
}
