#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "math/matrix.hpp"
#include "math/modint.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int n = 1; n <= 7; ++n) {
        for (int repetition = 0; repetition < 100; ++repetition) {
            poe::matrix<long long> base(n, n);
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j) base[i][j] = random() % 20;
            poe::matrix<long long> brute = poe::minplus_matrix_power(base, 0);
            for (int exponent = 0; exponent <= 10; ++exponent) {
                const auto result = poe::minplus_matrix_power(base, exponent);
                for (int i = 0; i < n; ++i)
                    for (int j = 0; j < n; ++j) assert(result[i][j] == brute[i][j]);
                brute = poe::minplus_matrix_product(brute, base);
            }
        }
    }

    using mint = poe::modint1000000007;
    for (int repetition = 0; repetition < 2000; ++repetition) {
        const int n = 1 + random() % 5;
        const int target = random() % 30;
        poe::matrix<mint> normal(n, n), exception(n, n);
        for (int row = 0; row < n; ++row) {
            for (int column = 0; column < n; ++column) {
                normal[row][column] = random() % 10;
                exception[row][column] = random() % 10;
            }
        }
        std::vector<mint> initial(n);
        for (auto& value : initial) value = random() % 10;
        std::vector<unsigned long long> positions;
        for (int time = 1; time <= target; ++time) {
            if (random() % 4 == 0) positions.push_back(time);
        }
        auto expected = initial;
        for (int time = 1; time <= target; ++time) {
            expected = poe::matrix_vector_product(
                std::ranges::binary_search(positions, static_cast<unsigned long long>(time))
                    ? exception : normal,
                expected
            );
        }
        auto actual = poe::sparse_matrix_transitions(
            initial, normal, exception, target, positions
        );
        assert(actual == expected);
    }
}
