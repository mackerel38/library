#pragma once
#include <bits/stdc++.h>
#include "math/modint.hpp"

namespace poe {

/// O(n^2 log B+n2^n)時間・O(2^n)領域。prod_i sum_{k=0}^{limit[i]}(value[i]x)^kの指定係数を返す。
template<int mod>
staticmodint<mod> bounded_geometric_product_coefficient(
    const std::vector<staticmodint<mod>>& values,
    const std::vector<long long>& limits,
    long long degree
) {
    using mint = staticmodint<mod>;
    const int size = static_cast<int>(values.size());
    assert(values.size() == limits.size());
    assert(0 <= size && size <= 24);
    assert(degree >= 0);
    for (int i = 0; i < size; ++i) {
        assert(0 <= limits[i] && limits[i] < std::numeric_limits<long long>::max());
        assert(values[i] != mint{0});
        for (int j = 0; j < i; ++j) assert(values[i] != values[j]);
    }
    if (size == 0) return degree == 0 ? mint{1} : mint{0};

    const int subsets = 1 << size;
    std::vector<long long> shift(subsets);
    for (int mask = 1; mask < subsets; ++mask) {
        const int bit = std::countr_zero(static_cast<unsigned>(mask));
        const int previous = mask & (mask - 1);
        const long long add = limits[bit] + 1;
        if (shift[previous] < 0 || add > degree - shift[previous]) shift[mask] = -1;
        else shift[mask] = shift[previous] + add;
    }

    mint answer = 0;
    std::vector<mint> subset_product(subsets);
    for (int pivot = 0; pivot < size; ++pivot) {
        mint partial_fraction = 1;
        std::vector<mint> factor(size);
        const mint inverse_pivot = values[pivot].inv();
        for (int index = 0; index < size; ++index) {
            if (index != pivot) partial_fraction *= mint{1} - values[index] * inverse_pivot;
            factor[index] = -(values[index] * inverse_pivot).pow(limits[index] + 1);
        }
        partial_fraction = partial_fraction.inv();

        subset_product[0] = 1;
        mint numerator_sum = 1;
        for (int mask = 1; mask < subsets; ++mask) {
            const int bit = std::countr_zero(static_cast<unsigned>(mask));
            subset_product[mask] = subset_product[mask & (mask - 1)] * factor[bit];
            if (shift[mask] >= 0) numerator_sum += subset_product[mask];
        }
        answer += partial_fraction * values[pivot].pow(degree) * numerator_sum;
    }
    return answer;
}

}
