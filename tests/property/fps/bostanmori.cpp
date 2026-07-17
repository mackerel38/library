#include <cassert>
#include <random>
#include <vector>

#include "fps/bostanmori.hpp"
#include "math/linearecurrence.hpp"

int main() {
    std::mt19937 random(20260715);
    using mint = poe::modint998244353;
    for (int test = 0; test < 100; ++test) {
        const int order = 1 + random() % 12;
        const int index = random() % 200;
        std::vector<mint> initial(order), coefficient(order);
        for (auto& x : initial) x = random() % 1000;
        for (auto& x : coefficient) x = random() % 1000;
        assert(poe::fast_linear_recurrence<998244353>(initial, coefficient, index) ==
               poe::linear_recurrence(initial, coefficient, index));
    }

    using mint1e9 = poe::modint1000000007;
    std::vector<mint1e9> initial(100), coefficient(100);
    for (auto& x : initial) x = random() % 1000;
    for (auto& x : coefficient) x = random() % 1000;
    assert(poe::fast_linear_recurrence<1000000007>(initial, coefficient, 250) ==
           poe::linear_recurrence(initial, coefficient, 250));
}
