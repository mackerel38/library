#include "math/convolution.hpp"
#include "math/math.hpp"
#include "math/modint.hpp"

int main() {
    assert(poe::pow_mod(2, 10, 1000) == 24);
    assert(poe::inv_mod(3, 11) == 4);
    assert(poe::crt({2, 3, 2}, {3, 5, 7}) == std::pair(23LL, 105LL));
    assert(poe::crt({0, 1}, {2, 2}) == std::pair(0LL, 0LL));
    assert(poe::floor_sum(4, 10, 6, 3) == 3);
    assert(poe::floor_sum(4, 10, -6, -3) == -7);

    using mint = poe::modint998244353;
    mint value = -1;
    assert(value.val() == 998244352);
    assert((mint(2).pow(10)).val() == 1024);
    assert(mint(3).inv() * 3 == 1);
    value = 10;
    value += 5;
    value *= 2;
    assert(value == 30);

    poe::modint::set_mod(11);
    poe::modint dynamic = 10;
    assert(dynamic + 3 == 2);
    assert(dynamic / 2 == 5);

    std::vector<int> left = {1, 2, 3};
    std::vector<int> right = {4, 5};
    assert(poe::convolution(left, right) == std::vector<int>({4, 13, 22, 15}));
    assert(poe::convolution_ll({-1, 2}, {3, -4}) ==
           std::vector<long long>({-3, 10, -8}));
}
