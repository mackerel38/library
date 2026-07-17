#include <cassert>
#include <random>

#include "fps/fps.hpp"

int main() {
    using fps = poe::fps998244353;
    using mint = poe::modint998244353;
    std::mt19937 random(20260715);

    for (int test = 0; test < 200; ++test) {
        const int size = 1 + random() % 50;
        fps f(size);
        f[0] = 1 + random() % 1000;
        for (int i = 1; i < size; ++i) f[i] = random() % 1000;

        const fps inverse = f.inv(size);
        const fps product = (f * inverse).prefix(size);
        assert(product[0] == 1);
        for (int i = 1; i < size; ++i) assert(product[i] == 0);

        fps unit = f / f[0];
        assert(unit.log(size).exp(size) == unit.prefix(size));

        fps zero(size);
        for (int i = 1; i < size; ++i) zero[i] = random() % 1000;
        assert(zero.exp(size).log(size) == zero);

        const int exponent = random() % 8;
        fps brute(size);
        brute[0] = 1;
        for (int count = 0; count < exponent; ++count) brute = (brute * f).prefix(size);
        assert(f.pow(exponent, size) == brute);
    }

    fps shifted = {0, 0, 3, 4};
    assert(shifted.pow(3, 8) == fps({0, 0, 0, 0, 0, 0, 27, 108}));
}
