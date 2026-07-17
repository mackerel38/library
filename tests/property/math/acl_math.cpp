#include "math/convolution.hpp"
#include "math/math.hpp"
#include "math/modint.hpp"

long long brute_floor_sum(long long n, long long mod, long long a, long long b) {
    long long result = 0;
    for (long long i = 0; i < n; ++i) {
        long long value = a * i + b;
        long long quotient = value / mod;
        if (value < 0 && value % mod != 0) {
            --quotient;
        }
        result += quotient;
    }
    return result;
}

int main() {
    std::mt19937_64 random(7654321);
    for (int mod = 1; mod <= 100; ++mod) {
        for (int iteration = 0; iteration < 200; ++iteration) {
            const long long base = static_cast<long long>(random() % 401) - 200;
            const int exponent = random() % 20;
            long long brute = 1 % mod;
            for (int i = 0; i < exponent; ++i) {
                brute = brute * ((base % mod + mod) % mod) % mod;
            }
            assert(poe::pow_mod(base, exponent, mod) == brute);
        }
    }

    for (int mod = 2; mod <= 200; ++mod) {
        for (int value = 1; value < mod; ++value) {
            if (std::gcd(value, mod) == 1) {
                assert(value * poe::inv_mod(value, mod) % mod == 1);
            }
        }
    }

    for (int n = 0; n <= 30; ++n) {
        for (int mod = 1; mod <= 30; ++mod) {
            for (int iteration = 0; iteration < 100; ++iteration) {
                const long long a = static_cast<long long>(random() % 101) - 50;
                const long long b = static_cast<long long>(random() % 101) - 50;
                assert(poe::floor_sum(n, mod, a, b) == brute_floor_sum(n, mod, a, b));
            }
        }
    }

    for (int n = 0; n <= 140; n += 7) {
        for (int m = 0; m <= 140; m += 11) {
            std::vector<long long> left(n), right(m);
            for (auto& value : left) {
                value = static_cast<long long>(random() % 2001) - 1000;
            }
            for (auto& value : right) {
                value = static_cast<long long>(random() % 2001) - 1000;
            }
            std::vector<long long> expected;
            if (n > 0 && m > 0) {
                expected.assign(n + m - 1, 0);
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < m; ++j) {
                        expected[i + j] += left[i] * right[j];
                    }
                }
            }
            assert(poe::convolution_ll(left, right) == expected);
            const auto modular = poe::convolution<998244353>(left, right);
            assert(modular.size() == expected.size());
            for (std::size_t i = 0; i < expected.size(); ++i) {
                long long normalized = expected[i] % 998244353;
                if (normalized < 0) {
                    normalized += 998244353;
                }
                assert(modular[i] == normalized);
            }
        }
    }
}
