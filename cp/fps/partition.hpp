#pragma once
#include <bits/stdc++.h>
#include "math/lucas.hpp"

namespace poe {

/// Euler積prod(1-x^i)の非零項x^degreeの係数を表す。
struct pentagonalterm {
    long long degree;
    int coefficient;
};

/// O(sqrt n)。prod_{i>=1}(1-x^i)のn次以下の非零項を次数順に返す。
inline std::vector<pentagonalterm> generalized_pentagonal_terms(
    long long maximum_degree
) {
    assert(maximum_degree >= 0);
    std::vector<pentagonalterm> result{{0, 1}};
    long long lower = 1;
    for (long long index = 1; lower <= maximum_degree; ++index) {
        const int coefficient = index % 2 == 0 ? 1 : -1;
        result.push_back({lower, coefficient});
        if (index <= maximum_degree - lower) {
            result.push_back({lower + index, coefficient});
        }
        const long long difference = 3 * index + 1;
        if (difference > maximum_degree - lower) break;
        lower += difference;
    }
    return result;
}

/// O(n sqrt n)。0<=s<=nの整数分割数p(s)を返す。answer[0]=1。
template <class T>
std::vector<T> partition_numbers(int maximum_sum) {
    assert(maximum_sum >= 0);
    const auto terms = generalized_pentagonal_terms(maximum_sum);
    std::vector<T> result(static_cast<std::size_t>(maximum_sum) + 1);
    result[0] = T{1};
    for (int sum = 1; sum <= maximum_sum; ++sum) {
        for (std::size_t index = 1;
             index < terms.size() && terms[index].degree <= sum; ++index) {
            result[sum] -= T{terms[index].coefficient} *
                           result[sum - terms[index].degree];
        }
    }
    return result;
}

/// O(p+sqrt d log_p(d+a))。素数pを法として[x^d]prod(1-x^i)/(1-x)^aを返す。
template <int prime>
staticmodint<prime> euler_product_over_one_minus_x_coefficient(
    long long degree,
    long long denominator_power
) {
    assert(degree >= 0 && denominator_power >= 0);
    using mint = staticmodint<prime>;
    if (denominator_power == 0) {
        const auto terms = generalized_pentagonal_terms(degree);
        const auto iterator = std::lower_bound(
            terms.begin(), terms.end(), degree,
            [](const pentagonalterm& term, long long target) {
                return term.degree < target;
            });
        return iterator != terms.end() && iterator->degree == degree
                   ? mint{iterator->coefficient}
                   : mint{0};
    }
    assert(degree <= std::numeric_limits<long long>::max() - denominator_power + 1);
    const lucascombination<prime> combination;
    mint result = 0;
    auto add_term = [&](long long term_degree, int coefficient) {
        const long long remaining = degree - term_degree;
        result += mint{coefficient} * combination.choose(
                                          denominator_power + remaining - 1,
                                          remaining);
    };
    add_term(0, 1);
    long long lower = 1;
    for (long long index = 1; lower <= degree; ++index) {
        const int coefficient = index % 2 == 0 ? 1 : -1;
        add_term(lower, coefficient);
        if (index <= degree - lower) {
            add_term(lower + index, coefficient);
        }
        const long long difference = 3 * index + 1;
        if (difference > degree - lower) break;
        lower += difference;
    }
    return result;
}

}
