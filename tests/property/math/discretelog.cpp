#include <cassert>

#include "math/discretelog.hpp"

int main() {
    for (int mod = 1; mod <= 80; ++mod) {
        for (int base = 0; base < mod; ++base) {
            for (int target = 0; target < mod; ++target) {
                long long brute = -1;
                long long value = 1 % mod;
                for (int exponent = 0; exponent <= mod * 2; ++exponent) {
                    if (value == target) { brute = exponent; break; }
                    value = value * base % mod;
                }
                assert(poe::discrete_log(base, target, mod) == brute);
            }
        }
    }
}
