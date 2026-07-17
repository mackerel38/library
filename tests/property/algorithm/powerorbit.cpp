#include <cassert>
#include "algorithm/powerorbit.hpp"

using namespace poe;

int main() {
    for (int modulus = 1; modulus <= 30; ++modulus) {
        for (int base = 0; base < modulus; ++base) {
            finitepowerorbit orbit(1 % modulus, base, [=](int left, int right) {
                return left * right % modulus;
            });
            int brute = 1 % modulus;
            for (int exponent = 0; exponent <= 1000; ++exponent) {
                assert(orbit[exponent] == brute);
                brute = brute * base % modulus;
            }
        }
    }
}
