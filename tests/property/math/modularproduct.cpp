#include <bits/stdc++.h>
#include "math/modularproduct.hpp"

using namespace std;
using namespace poe;

int main() {
    constexpr int result_mod = 998244353;
    for (int modulus = 1; modulus <= 40; ++modulus) {
        for (int length = 0; length <= 9; ++length) {
            vector<modint998244353> count(modulus), next(modulus);
            count[1 % modulus] = 1;
            for (int step = 0; step < length; ++step) {
                fill(next.begin(), next.end(), 0);
                for (int product = 0; product < modulus; ++product) {
                    for (int value = 0; value < modulus; ++value) {
                        next[product * value % modulus] += count[product];
                    }
                }
                count.swap(next);
            }
            for (int target = 0; target < modulus; ++target) {
                assert(count_modular_product_sequences<result_mod>(
                    length, target, modulus) == count[target]);
            }
        }
    }
}
