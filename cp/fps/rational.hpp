#pragma once
#include <bits/stdc++.h>
#include "fps/bostanmori.hpp"
#include "fps/fps.hpp"

namespace poe {

/// rationalfps<mod>{p,q}: 有理型母関数p(x)/q(x)を表す。q[0]!=0。
template<int mod>
struct rationalfps {
    fps<mod> numerator;
    fps<mod> denominator{1};

    /// O(M(n))。left * right: 有理式の積を返す。
    friend rationalfps operator*(const rationalfps& left, const rationalfps& right) {
        return {left.numerator * right.numerator, left.denominator * right.denominator};
    }

    /// O(M(n))。left + right: 有理式の和を返す。
    friend rationalfps operator+(const rationalfps& left, const rationalfps& right) {
        return {
            left.numerator * right.denominator + right.numerator * left.denominator,
            left.denominator * right.denominator
        };
    }

    /// O(M(n) log n)。f.prefix(n): 有理式をx^n未満のFPSへ展開する。
    fps<mod> prefix(int size) const { return numerator.div(denominator, size); }

    /// O(M(k) log index)。f.coeff(index): 有理式のx^indexの係数を返す。
    staticmodint<mod> coeff(long long index) const {
        assert(index >= 0 && !denominator.empty() && denominator[0] != 0);
        auto quotient_remainder = polynomial_reduce();
        if (index < quotient_remainder.first.size_int()) {
            return quotient_remainder.first[static_cast<int>(index)]
                + bostan_mori<mod>(quotient_remainder.second, denominator, index);
        }
        return bostan_mori<mod>(quotient_remainder.second, denominator, index);
    }

    /// O(M(k) log mask)。係数a[i]についてiがmaskのsubmaskであるa[i]の総和を返す。
    staticmodint<mod> submask_sum(unsigned long long mask) const {
        assert(!denominator.empty() && denominator[0] != 0);
        const auto quotient_remainder = polynomial_reduce();
        staticmodint<mod> answer{};
        for (int index = 0; index < quotient_remainder.first.size_int(); ++index) {
            if ((static_cast<unsigned long long>(index) & mask)
                == static_cast<unsigned long long>(index)) {
                answer += quotient_remainder.first[index];
            }
        }
        return answer + bostan_mori_submask_sum<mod>(
            quotient_remainder.second,
            denominator,
            mask
        );
    }

private:
    std::pair<fps<mod>, fps<mod>> polynomial_reduce() const {
        fps<mod> quotient;
        fps<mod> remainder = numerator;
        if (numerator.size() >= denominator.size()) {
            fps<mod> reversed_numerator(numerator.rbegin(), numerator.rend());
            fps<mod> reversed_denominator(denominator.rbegin(), denominator.rend());
            const int size = numerator.size_int() - denominator.size_int() + 1;
            quotient = (reversed_numerator.prefix(size) * reversed_denominator.inv(size)).prefix(size);
            std::reverse(quotient.begin(), quotient.end());
            remainder = numerator - denominator * quotient;
            remainder.resize(denominator.size() - 1);
        }
        quotient.shrink();
        remainder.shrink();
        return {std::move(quotient), std::move(remainder)};
    }
};

/// O(M(S) log k)。rational_sum(terms): 有理式列の和を分割統治で返す。空和は0。
template<int mod>
rationalfps<mod> rational_sum(std::vector<rationalfps<mod>> terms) {
    if (terms.empty()) return {{}, {1}};
    auto solve = [&](auto&& self, int left, int right) -> rationalfps<mod> {
        if (right - left == 1) return terms[left];
        const int middle = (left + right) / 2;
        return self(self, left, middle) + self(self, middle, right);
    };
    return solve(solve, 0, static_cast<int>(terms.size()));
}

/// O(M(S) log k)。rational_product(terms): 有理式列の積を分割統治で返す。空積は1。
template<int mod>
rationalfps<mod> rational_product(std::vector<rationalfps<mod>> terms) {
    if (terms.empty()) return {{1}, {1}};
    auto solve = [&](auto&& self, int left, int right) -> rationalfps<mod> {
        if (right - left == 1) return std::move(terms[left]);
        const int middle = std::midpoint(left, right);
        return self(self, left, middle) * self(self, middle, right);
    };
    return solve(solve, 0, static_cast<int>(terms.size()));
}

/// weighted_sum_rationalへ渡す変数。weight*xを加え、limit<0ならx>=0、他は0<=x<=limit。
struct weightedsumvariable {
    int weight;
    long long limit = -1;
};

/// O(M(S) log k)。独立な非負整数変数の重み付き和を数える母関数を制約から構築する。
template<int mod>
rationalfps<mod> weighted_sum_rational(const std::vector<weightedsumvariable>& variables) {
    using series = fps<mod>;
    std::vector<rationalfps<mod>> factors;
    factors.reserve(variables.size());
    for (const auto [weight, limit] : variables) {
        assert(weight > 0);
        if (limit < 0) {
            series denominator(static_cast<std::size_t>(weight) + 1);
            denominator[0] = 1;
            denominator[weight] = -1;
            factors.push_back({series{1}, std::move(denominator)});
        } else {
            const __int128 degree = static_cast<__int128>(weight) * limit;
            assert(degree <= std::numeric_limits<int>::max() - 1);
            series numerator(static_cast<std::size_t>(degree) + 1);
            for (long long value = 0; value <= limit; ++value) {
                numerator[static_cast<std::size_t>(value) * weight] = 1;
            }
            factors.push_back({std::move(numerator), series{1}});
        }
    }
    return rational_product(std::move(factors));
}

/// O(M(S) log k + M(S) log target)。sum weight[i]*x[i]=targetとなる制約付き非負整数組を数える。
template<int mod>
staticmodint<mod> count_weighted_sum(
    long long target,
    const std::vector<weightedsumvariable>& variables
) {
    assert(target >= 0);
    return weighted_sum_rational<mod>(variables).coeff(target);
}

/// O(n+d log d+M(d)log d+M(size))。weighted_power_sums(values,weights,size)[k]=sum weights[i]*values[i]^k。
template<int mod>
fps<mod> weighted_power_sums(
    const std::vector<staticmodint<mod>>& values,
    const std::vector<staticmodint<mod>>& weights,
    int size
) {
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    assert(values.size() == weights.size() && size >= 0);
    std::map<int, mint> combined;
    for (int index = 0; index < static_cast<int>(values.size()); ++index) {
        combined[values[index].val()] += weights[index];
    }
    std::vector<rationalfps<mod>> terms;
    terms.reserve(combined.size());
    for (const auto [value, weight] : combined) {
        if (weight != mint{}) {
            terms.push_back({series{weight}, series{1, -mint{value}}});
        }
    }
    if (terms.empty()) return series(static_cast<std::size_t>(size));
    return rational_sum(std::move(terms)).prefix(size);
}

/// O(n+d log d+M(d)log d+M(size))。power_sums(values,size)[k]=sum values[i]^kを返す。
template<int mod>
fps<mod> power_sums(const std::vector<staticmodint<mod>>& values, int size) {
    return weighted_power_sums<mod>(
        values,
        std::vector<staticmodint<mod>>(values.size(), staticmodint<mod>{1}),
        size
    );
}

}
