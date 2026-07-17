#include <cassert>
#include <random>
#include <string>
#include <vector>

#include "math/decimal.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int trial = 0; trial < 2000; ++trial) {
        std::vector<poe::decimalrun> runs;
        std::string digits;
        const int count = 1 + random() % 4;
        for (int i = 0; i < count; ++i) {
            const int digit = random() % 10;
            const int length = 1 + random() % 3;
            runs.push_back({digit, static_cast<unsigned long long>(length)});
            digits.append(length, static_cast<char>('0' + digit));
        }
        unsigned long long value = 0;
        for (char digit : digits) value = value * 10 + digit - '0';
        const unsigned long long modulus = 1 + random() % 10000;
        const unsigned long long divisor = 1 + random() % 100;
        assert(poe::decimal_runs_mod(runs, modulus) == value % modulus);
        assert(poe::decimal_runs_div_mod(runs, divisor, modulus) ==
               (value / divisor) % modulus);
    }
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = 1 + random() % 30;
        const unsigned long long base = 2 + random() % 15;
        const unsigned long long modulus = 1 + random() % 1000;
        std::vector<unsigned long long> values(n);
        for (auto& value : values) value = 1 + random() % 100000;
        long long expected = 0;
        for (auto left : values) for (auto right : values) {
            unsigned long long power = 1;
            for (auto copy = right; copy >= base; copy /= base) power *= base;
            power *= base;
            expected += (static_cast<unsigned __int128>(left) * power + right) % modulus == 0;
        }
        assert(poe::count_divisible_concatenations(values, modulus, base) == expected);
    }
    for (int modulus = 1; modulus <= 100; ++modulus) {
        for (int value = 1; value <= 99; ++value) {
            const std::string pattern = std::to_string(value);
            long long expected = modulus;
            while (std::to_string(expected).find(pattern) == std::string::npos) expected += modulus;
            assert(poe::smallest_multiple_containing(modulus, pattern) == std::to_string(expected));
        }
    }
}
