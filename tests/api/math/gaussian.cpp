#include <cassert>

#include "math/gaussian.hpp"
#include "math/modint.hpp"

int main() {
    using mint = poe::modint998244353;
    poe::matrix<mint> coefficients({{1, 1}, {1, -1}});
    const auto result = poe::solve_linear(coefficients, std::vector<mint>{3, 1});
    assert(result.status == poe::linearstatus::unique);
    assert(result[0] == 2 && result[1] == 1);
    assert(poe::determinant(coefficients) == -2);
}
