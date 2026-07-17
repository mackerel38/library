#include <cassert>

#include "math/prime.hpp"

int main() {
    constexpr int limit = 100000;
    poe::primetable table(limit);
    for (int value = 1; value <= limit; ++value) {
        bool brute = value >= 2;
        for (int divisor = 2; 1LL * divisor * divisor <= value; ++divisor) {
            if (value % divisor == 0) brute = false;
        }
        assert(table[value] == brute);
        assert(poe::is_prime(value) == brute);
        unsigned long long product = 1;
        for (const auto [prime, exponent] : poe::factorize(static_cast<unsigned long long>(value))) {
            assert(poe::is_prime(prime));
            for (int count = 0; count < exponent; ++count) product *= prime;
        }
        assert(product == static_cast<unsigned long long>(value));

        if (value <= 300) {
            const auto signed_values = poe::signed_divisors(-value);
            std::vector<long long> brute_divisors;
            for (int divisor = -value; divisor <= value; ++divisor) {
                if (divisor != 0 && value % divisor == 0) brute_divisors.push_back(divisor);
            }
            assert(signed_values == brute_divisors);
        }
    }
}
