#pragma once
#include <bits/stdc++.h>
#include "math/prime.hpp"
#include "structure/mo.hpp"

namespace poe {

/// O(A+n log A+(n+q)sqrt(q)log A)。各区間の積がexponent乗数かを厳密に判定する。
inline std::vector<bool> range_product_power_queries(
    const std::vector<int>& values,
    const std::vector<std::pair<int, int>>& queries,
    int exponent
) {
    assert(exponent >= 2);
    const int n = static_cast<int>(values.size());
    const int maximum = values.empty() ? 1 : *std::ranges::max_element(values);
    assert(maximum >= 1);
    primetable primes(maximum);
    std::vector<std::vector<std::pair<int, int>>> factors(n);
    for (int index = 0; index < n; ++index) {
        assert(values[index] >= 1);
        factors[index] = primes.factorize(values[index]);
        for (auto& [prime, count] : factors[index]) count %= exponent;
    }

    Mo mo(n);
    for (const auto& [left, right] : queries) mo.add(left, right);
    std::vector<int> residue(maximum + 1);
    int nonzero = 0;
    auto change = [&](int index, int sign) {
        for (const auto [prime, count] : factors[index]) {
            nonzero -= residue[prime] != 0;
            residue[prime] = (residue[prime] + sign * count) % exponent;
            if (residue[prime] < 0) residue[prime] += exponent;
            nonzero += residue[prime] != 0;
        }
    };
    std::vector<bool> answer(queries.size());
    mo.run(
        [&](int index) { change(index, 1); },
        [&](int index) { change(index, -1); },
        [&](int id) { answer[id] = nonzero == 0; }
    );
    return answer;
}

}
