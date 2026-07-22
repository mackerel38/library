#pragma once
#include <bits/stdc++.h>
#include "algorithm/powerorbit.hpp"
#include "math/gaussianinteger.hpp"

namespace poe {

/// recipe。O(r C^4 + sum sqrt(p))。norm(z)=Nとなるガウス整数zの法Cでの剰余分布を返す。
template <class Mint>
std::vector<Mint> gaussian_norm_residue_distribution(
    const std::vector<std::pair<int, unsigned long long>>& factorization,
    int modulus
) {
    assert(modulus >= 1);
    using gi = gaussianinteger<long long>;
    auto multiply = [=](const gi& left, const gi& right) {
        return gaussian_mod(left * right, static_cast<long long>(modulus));
    };
    auto power = [&](gi base, unsigned long long exponent) {
        gi result{1 % modulus, 0};
        while (exponent > 0) {
            if (exponent & 1) result = multiply(result, base);
            exponent >>= 1;
            if (exponent > 0) base = multiply(base, base);
        }
        return result;
    };
    auto index = [=](const gi& value) {
        return static_cast<int>(value.real * modulus + value.imaginary);
    };

    gi fixed{1 % modulus, 0};
    std::vector<std::pair<int, unsigned long long>> split_primes;
    for (const auto& [prime, exponent] : factorization) {
        assert(prime >= 2 && exponent >= 1);
        if (prime == 2) {
            fixed = multiply(fixed, power({1, 1}, exponent));
        } else if (prime % 4 == 3) {
            if (exponent & 1) return std::vector<Mint>(modulus * modulus);
            fixed = multiply(fixed, power({prime % modulus, 0}, exponent / 2));
        } else {
            split_primes.emplace_back(prime, exponent);
        }
    }

    const int state_count = modulus * modulus;
    std::vector<int> product(static_cast<std::size_t>(state_count) * state_count);
    for (int first = 0; first < state_count; ++first) {
        const gi left{first / modulus, first % modulus};
        for (int second = 0; second < state_count; ++second) {
            const gi right{second / modulus, second % modulus};
            product[static_cast<std::size_t>(first) * state_count + second] =
                index(multiply(left, right));
        }
    }

    std::vector<Mint> distribution(state_count);
    gi unit_multiple = fixed;
    for (int unit = 0; unit < 4; ++unit) {
        distribution[index(unit_multiple)] += Mint{1};
        unit_multiple = multiply(unit_multiple, {0, 1 % modulus});
    }

    for (const auto& [prime, exponent] : split_primes) {
        int x = 0, y = 0;
        for (int candidate = 1; 1LL * candidate * candidate <= prime; ++candidate) {
            const int remainder = prime - candidate * candidate;
            const int root = static_cast<int>(std::sqrt(remainder));
            if (root * root == remainder) {
                x = candidate;
                y = root;
                break;
            }
        }
        assert(x != 0 || y != 0);
        const gi gaussian_prime{x % modulus, y % modulus};
        finitepowerorbit scalar_orbit(
            1LL % modulus,
            static_cast<long long>(prime % modulus),
            [=](long long left, long long right) { return left * right % modulus; }
        );
        finitepowerorbit gaussian_orbit(gi{1 % modulus, 0}, gaussian_prime, multiply);

        std::vector<Mint> factor_distribution(state_count);
        auto add_choice = [&](unsigned long long choice, Mint frequency) {
            const gi power_part = gaussian_orbit[exponent - 2 * choice];
            const gi value = multiply({scalar_orbit[choice], 0}, power_part);
            factor_distribution[index(value)] += frequency;
            factor_distribution[index(gaussian_mod(
                value.conjugate(), static_cast<long long>(modulus)))] += frequency;
        };

        const unsigned long long half = exponent / 2;
        const unsigned long long end = half + (exponent & 1);
        const unsigned long long prefix_end = std::min<unsigned long long>(
            end, scalar_orbit.preperiod());
        for (unsigned long long choice = 0; choice < prefix_end; ++choice) {
            add_choice(choice, Mint{1});
        }

        unsigned long long middle_end = prefix_end;
        if (exponent >= static_cast<unsigned long long>(gaussian_orbit.preperiod())) {
            middle_end = std::min(
                end,
                (exponent - gaussian_orbit.preperiod()) / 2 + 1
            );
        }
        const unsigned long long middle_begin = prefix_end;
        if (middle_begin < middle_end) {
            const unsigned long long gaussian_step_period =
                gaussian_orbit.period() / std::gcd(gaussian_orbit.period(), 2);
            const unsigned long long period = std::lcm<unsigned long long>(
                scalar_orbit.period(), gaussian_step_period);
            const unsigned long long length = middle_end - middle_begin;
            const unsigned long long full = length / period;
            const unsigned long long remainder = length % period;
            const unsigned long long enumerate = std::min(length, period);
            for (unsigned long long offset = 0; offset < enumerate; ++offset) {
                add_choice(middle_begin + offset, Mint(full + (offset < remainder)));
            }
        }
        for (unsigned long long choice = std::max(prefix_end, middle_end);
             choice < end; ++choice) {
            add_choice(choice, Mint{1});
        }
        if ((exponent & 1) == 0) {
            factor_distribution[index({scalar_orbit[half], 0})] += Mint{1};
        }

        std::vector<std::pair<int, Mint>> left, right;
        for (int state = 0; state < state_count; ++state) {
            if (distribution[state] != Mint{}) left.emplace_back(state, distribution[state]);
            if (factor_distribution[state] != Mint{}) {
                right.emplace_back(state, factor_distribution[state]);
            }
        }
        std::vector<Mint> next(state_count);
        for (const auto& [first, first_count] : left) {
            for (const auto& [second, second_count] : right) {
                next[product[static_cast<std::size_t>(first) * state_count + second]] +=
                    first_count * second_count;
            }
        }
        distribution = std::move(next);
    }
    return distribution;
}

}
