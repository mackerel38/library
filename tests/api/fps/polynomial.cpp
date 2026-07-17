#include <cassert>

#include "fps/polynomial.hpp"

int main() {
    using fps = poe::fps998244353;
    using mint = poe::modint998244353;

    assert(poe::polynomial_product<998244353>({fps{1, 1}, fps{1, 2}}) == fps({1, 3, 2}));
    poe::matrix<fps> first({{fps{1, 1}, fps{1}}, {fps{}, fps{1}}});
    poe::matrix<fps> second({{fps{1}, fps{0, 1}}, {fps{1}, fps{1}}});
    const auto matrix_product = poe::polynomial_matrix_product<998244353>(
        {first, second}, 4
    );
    const auto expected_matrix_product = first * second;
    for (int row = 0; row < 2; ++row) {
        for (int column = 0; column < 2; ++column) {
            auto expected = expected_matrix_product[row][column];
            expected.shrink();
            assert(matrix_product[row][column] == expected);
        }
    }
    const auto [quotient, remainder] = poe::polynomial_divmod(fps{1, 3, 2}, fps{1, 1});
    assert(quotient == fps({1, 2}));
    assert(remainder.empty());

    const std::vector<mint> points = {0, 1, 2, 3};
    const fps f = {5, 2, 3, 4};
    const auto values = poe::multipoint_evaluation(f, points);
    assert(values[0] == 5 && values[1] == 14 && values[2] == 53 && values[3] == 146);
    assert(poe::polynomial_interpolation<998244353>(points, values) == f);
    assert(poe::taylor_shift(fps{1, 2, 3}, mint{4}) == fps({57, 26, 3}));
}
