#include <cassert>
#include <random>

#include "fps/polynomial.hpp"

int main() {
    using fps = poe::fps998244353;
    using mint = poe::modint998244353;
    std::mt19937 random(20260716);
    for (int test = 0; test < 100; ++test) {
        const int count = 1 + random() % 8;
        std::vector<poe::matrix<fps>> matrices;
        for (int index = 0; index < count; ++index) {
            poe::matrix<fps> current(2, 2);
            for (int row = 0; row < 2; ++row) {
                for (int column = 0; column < 2; ++column) {
                    current[row][column] = {
                        mint{random() % 20}, mint{random() % 20}
                    };
                }
            }
            matrices.push_back(std::move(current));
        }
        auto brute = matrices[0];
        for (int index = 1; index < count; ++index) brute = brute * matrices[index];
        const int limit = 1 + random() % (count + 2);
        const auto fast = poe::polynomial_matrix_product<998244353>(matrices, limit);
        for (int row = 0; row < 2; ++row) {
            for (int column = 0; column < 2; ++column) {
                auto expected = brute[row][column].prefix(limit);
                expected.shrink();
                assert(fast[row][column] == expected);
            }
        }
    }
    for (int test = 0; test < 100; ++test) {
        const int degree = 1 + random() % 30;
        fps f(degree);
        for (auto& value : f) value = random() % 100000;
        f.back() = 1 + random() % 100000;

        std::vector<mint> points(degree);
        for (int i = 0; i < degree; ++i) points[i] = i + 100;
        const auto values = poe::multipoint_evaluation(f, points);
        for (int i = 0; i < degree; ++i) assert(values[i] == f.eval(points[i]));
        assert(poe::polynomial_interpolation<998244353>(points, values) == f);

        fps divisor(1 + random() % degree);
        for (auto& value : divisor) value = random() % 100000;
        divisor.back() = 1 + random() % 100000;
        const auto [quotient, remainder] = poe::polynomial_divmod(f, divisor);
        fps restored = quotient * divisor + remainder;
        restored.shrink();
        assert(restored == f);

        const mint shift = random() % 1000;
        const fps moved = poe::taylor_shift(f, shift);
        for (int i = 0; i < 10; ++i) {
            const mint x = random() % 1000;
            assert(moved.eval(x) == f.eval(x + shift));
        }
    }
}
