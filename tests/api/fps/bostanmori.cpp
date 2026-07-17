#include "fps/bostanmori.hpp"

int main() {
    using mint = poe::modint998244353;
    const std::vector<mint> initial = {0, 1};
    const std::vector<mint> coefficient = {1, 1};
    assert(poe::fast_linear_recurrence<998244353>(initial, coefficient, 10) == 55);

    using mint1e9 = poe::modint1000000007;
    std::vector<mint1e9> long_initial(100, 1);
    std::vector<mint1e9> long_coefficient(100, 1);
    assert(poe::fast_linear_recurrence<1000000007>(long_initial, long_coefficient, 100) == 100);
}
