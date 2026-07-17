#include <cassert>
#include <random>

#include "fps/powerprojection.hpp"

int main() {
    using fps = poe::fps998244353;
    std::mt19937 random(20260716);
    for (int trial = 0; trial < 400; ++trial) {
        const int size = 1 + random() % 18;
        fps function(size), multiplier(size);
        for (auto& value : function) value = random() % 11;
        for (auto& value : multiplier) value = random() % 11;
        const auto actual = poe::power_projection(function, multiplier);
        fps power{1};
        for (int exponent = 0; exponent < size; ++exponent) {
            const auto product = power * multiplier;
            assert(actual[exponent] == product.coeff(size - 1));
            power = (power * function).prefix(size);
        }
    }
}
