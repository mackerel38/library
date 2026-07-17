#include <cassert>
#include <random>

#include "math/modularsum.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937_64 random(20260716);
    for (int repetition = 0; repetition < 100000; ++repetition) {
        const unsigned long long n = 1 + random() % 30;
        const unsigned long long m = 1 + random() % 30;
        const unsigned long long a = random() % m;
        const unsigned long long b = random() % m;
        const int modulus = 1 + random() % 50;
        const unsigned long long base = random() % modulus;
        long long expected = 0;
        for (unsigned long long i = 0; i < n; ++i) {
            expected += pow_mod(base, (a * i + b) % m, modulus);
            expected %= modulus;
        }
        assert(sum_power_mod_linear(n, m, a, b, base, modulus) == expected);
    }
}
