#include <cassert>
#include <map>
#include <random>
#include "math/modint.hpp"
#include "structure/affinemap.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    std::mt19937 random(20260716);
    poe::affinemap<int, mint> actual;
    std::map<int, mint> expected;
    for (int query = 0; query < 3000; ++query) {
        const int type = random() % 3;
        const int key = random() % 30;
        if (type == 0) {
            const mint value = random();
            actual.set(key, value);
            expected[key] = value;
        } else if (type == 1) {
            assert(actual.erase(key) == (expected.erase(key) != 0));
        } else {
            const mint scale = 1 + random() % 20;
            const mint shift = random();
            actual.apply(scale, shift);
            for (auto& [_, value] : expected) value = scale * value + shift;
        }
        mint sum = 0;
        for (const auto& [key, value] : expected) {
            assert(actual.contains(key));
            assert(actual.get(key) == value);
            sum += value;
        }
        assert(actual.size() == static_cast<int>(expected.size()));
        assert(actual.sum() == sum);
    }
}
