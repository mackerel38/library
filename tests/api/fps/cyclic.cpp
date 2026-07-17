#include <cassert>

#include "fps/cyclic.hpp"

int main() {
    using mint = poe::modint998244353;
    std::vector<mint> step(8);
    step[1] = step[7] = 1;
    const auto result = poe::cyclic_power<998244353>(step, 2);
    assert(result[0] == 2 && result[2] == 1 && result[6] == 1);
    const std::vector<mint> left{1, 2, 3}, right{4, 5, 6};
    assert(poe::cyclic_convolution<998244353>(left, right)
        == std::vector<mint>({31, 31, 28}));
    assert(poe::cyclic_power<998244353>(left, 0)
        == std::vector<mint>({1, 0, 0}));
}
