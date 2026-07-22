#include "math/multiconvolution.hpp"

using namespace poe;

int main() {
    using mint = modint998244353;
    std::vector<std::vector<mint>> base{{1, 2}, {3, 4}};
    const auto square = sum_xor_convolution_power<998244353>(base, 2);
    assert(square.size() == 3 && square[0].size() == 2);
    assert(square[0][0] == 5 && square[0][1] == 4);
    assert(square[1][0] == 22 && square[1][1] == 20);
    assert(square[2][0] == 25 && square[2][1] == 24);
}
