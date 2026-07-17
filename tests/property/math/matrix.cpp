#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "math/matrix.hpp"

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
}
