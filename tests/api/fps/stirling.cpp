#include "fps/stirling.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    assert(poe::signed_stirling_first_row<998244353>(4) ==
           std::vector<mint>({0, -6, 11, -6, 1}));
    assert(poe::stirling_second_row<998244353>(5) ==
           std::vector<mint>({0, 1, 15, 25, 10, 1}));
    assert(poe::falling_factorial_polynomial<998244353>(0) ==
           std::vector<mint>({1}));
    assert(poe::unsigned_stirling_first_row<998244353>(4) ==
           std::vector<mint>({0, 6, 11, 6, 1}));
    assert(poe::block_permutation_cycle_counts<998244353>({2, 3}) ==
           std::vector<mint>({0, 0, 2, 5, 4, 1}));
}
