#include <cassert>
#include <random>

#include "fps/sparse.hpp"
#include "fps/transform.hpp"

int main() {
    using fps = poe::fps998244353;
    using mint = poe::modint998244353;
    std::mt19937 random(20260718);
    for (int test = 0; test < 100; ++test) {
        const int size = 1 + random() % 100;
        fps f(size);
        for (int count = 0; count < 5; ++count) f[random() % size] = random() % 1000;
        const int exponent = random() % 10;
        assert(poe::sparse_power<998244353>(f, exponent, size) == f.pow(exponent, size));

        fps values(size);
        for (auto& value : values) value = random() % 1000;
        const mint coefficient = random() % 1000;
        const fps transformed = poe::binomial_transform<998244353>(values, coefficient);
        for (int n = 0; n < size; ++n) {
            mint expected{}, choose = 1;
            for (int k = 0; k <= n; ++k) {
                expected += choose * values[k] * coefficient.pow(n - k);
                if (k < n) choose = choose * (n - k) / (k + 1);
            }
            assert(transformed[n] == expected);
        }
        const fps upper = poe::upper_binomial_transform<998244353>(values, coefficient);
        for (int k = 0; k < size; ++k) {
            mint expected{}, choose = 1;
            for (int n = k; n < size; ++n) {
                expected += choose * values[n] * coefficient.pow(n - k);
                choose = choose * (n + 1) / (n + 1 - k);
            }
            assert(upper[k] == expected);
        }

        fps other(size);
        for (auto& value : other) value = random() % 1000;
        const fps combined = poe::binomial_convolution<998244353>(values, other);
        for (int n = 0; n < combined.size_int(); ++n) {
            mint expected{}, choose = 1;
            for (int k = 0; k <= n; ++k) {
                expected += choose * values.coeff(k) * other.coeff(n - k);
                if (k < n) choose = choose * (n - k) / (k + 1);
            }
            assert(combined[n] == expected);
        }
    }
}
