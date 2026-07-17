#include <cassert>

#include "fps/setfps.hpp"

int main() {
    using mint = poe::modint998244353;
    std::vector<mint> connected(8);
    connected[1] = connected[2] = connected[4] = 1;
    const auto all = poe::setfps_exp<998244353>(connected);
    assert(all[7] == 1);
    assert(poe::setfps_log<998244353>(all) == connected);
}
