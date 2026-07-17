#include <cassert>
#include <random>

#include "math/gaussian.hpp"
#include "math/modint.hpp"

int main() {
    using mint = poe::modint998244353;
    std::mt19937 random(20260724);
    for (int test = 0; test < 500; ++test) {
        const int equations = random() % 8;
        const int variables = random() % 8;
        poe::matrix<mint> coefficients(equations, variables);
        std::vector<mint> expected(variables), right(equations);
        for (auto& value : expected) value = random() % 100;
        for (int row = 0; row < equations; ++row) {
            for (int column = 0; column < variables; ++column) {
                coefficients[row][column] = random() % 100;
                right[row] += coefficients[row][column] * expected[column];
            }
        }
        const auto result = poe::solve_linear(coefficients, right);
        assert(result.status != poe::linearstatus::none);
        auto check = [&](const std::vector<mint>& value, bool homogeneous) {
            for (int row = 0; row < equations; ++row) {
                mint sum{};
                for (int column = 0; column < variables; ++column) sum += coefficients[row][column] * value[column];
                assert(sum == (homogeneous ? mint{} : right[row]));
            }
        };
        check(result.solution, false);
        for (const auto& direction : result.basis) check(direction, true);
        assert(static_cast<int>(result.basis.size()) == variables - result.rank);
        assert(result.rank == poe::matrix_rank(coefficients));
    }
}
