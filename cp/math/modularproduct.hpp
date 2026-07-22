#pragma once
#include <bits/stdc++.h>
#include "math/combination.hpp"
#include "math/modint.hpp"
#include "math/prime.hpp"

namespace poe {

/// 期待O(M^(1/4)+omega(M)log M)。0<=a_i<Mかつ積a_i≡target(mod M)となる長さKの列を数える。
template<int mod>
staticmodint<mod> count_modular_product_sequences(
    long long length,
    unsigned long long target,
    unsigned long long modulus
) {
    using mint = staticmodint<mod>;
    assert(length >= 0);
    assert(modulus >= 1 && target < modulus);
    if (length == 0) return target == 1 % modulus ? mint{1} : mint{0};

    mint answer = 1;
    for (const auto [prime, exponent] : factorize(modulus)) {
        assert(exponent < mod);
        unsigned long long prime_power = 1;
        for (int count = 0; count < exponent; ++count) prime_power *= prime;

        const unsigned long long local_target = target % prime_power;
        int valuation = 0;
        unsigned long long reduced = local_target;
        while (valuation < exponent && reduced % prime == 0) {
            reduced /= prime;
            ++valuation;
        }

        const mint phi = mint{prime_power / prime} * mint{prime - 1};
        combination<mint> small(exponent);
        const auto compositions = [&](int sum) {
            return small.choose_large(length + sum - 1, sum);
        };

        if (valuation < exponent) {
            answer *= phi.pow(length - 1) * compositions(valuation);
            continue;
        }

        mint nonzero_products = 0;
        unsigned long long class_count = prime_power / prime * (prime - 1);
        const mint common = phi.pow(length - 1);
        for (int sum = 0; sum < exponent; ++sum) {
            nonzero_products += common * compositions(sum) * mint{class_count};
            class_count /= prime;
        }
        answer *= mint{prime_power}.pow(length) - nonzero_products;
    }
    return answer;
}

}  // namespace poe
