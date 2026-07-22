#pragma once
#include <bits/stdc++.h>
#include "fps/polynomial.hpp"

namespace poe {

/// subset_sum_parity_countsの結果。even[s], odd[s]は和sを作る偶数個・奇数個の部分集合数。
template <int mod>
struct subsetsumparitycounts {
    std::vector<staticmodint<mod>> even;
    std::vector<staticmodint<mod>> odd;
};

/// O(n + M(T) log d)時間・O(T)領域。各和を作る部分集合数を選択個数の偶奇別に返す。
/// valuesは非負、0<=limitかつlimit+1<mod、modは奇素数であること。
/// dはlimit以下の異なる正の値の個数、T=sum_v min(limit,count[v]*v)。
template <int mod>
subsetsumparitycounts<mod> subset_sum_parity_counts(
    const std::vector<int>& values,
    int limit
) {
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    assert(limit >= 0 && limit + 1 < mod);
    static_assert(mod % 2 == 1);

    std::vector<int> frequency(static_cast<std::size_t>(limit) + 1);
    for (const int value : values) {
        assert(value >= 0);
        if (value <= limit) ++frequency[value];
    }

    std::vector<series> positive_factors;
    std::vector<series> negative_factors;
    for (int value = 1; value <= limit; ++value) {
        if (frequency[value] == 0) continue;
        const int terms = std::min(frequency[value], limit / value);
        series positive(static_cast<std::size_t>(terms * value) + 1);
        series negative(static_cast<std::size_t>(terms * value) + 1);
        mint choose = 1;
        for (int selected = 0; selected <= terms; ++selected) {
            positive[selected * value] = choose;
            negative[selected * value] = (selected & 1) ? -choose : choose;
            if (selected != terms) {
                choose *= mint{frequency[value] - selected};
                choose /= mint{selected + 1};
            }
        }
        positive_factors.push_back(std::move(positive));
        negative_factors.push_back(std::move(negative));
    }

    series positive = polynomial_product<mod>(std::move(positive_factors), limit + 1);
    series negative(static_cast<std::size_t>(limit) + 1);
    if (frequency[0] == 0) {
        negative = polynomial_product<mod>(std::move(negative_factors), limit + 1);
    }
    positive.resize(static_cast<std::size_t>(limit) + 1);
    negative.resize(static_cast<std::size_t>(limit) + 1);
    const mint zero_factor = mint{2}.pow(frequency[0]);
    for (mint& coefficient : positive) coefficient *= zero_factor;

    const mint inverse_two = mint{2}.inv();
    subsetsumparitycounts<mod> result{
        std::vector<mint>(static_cast<std::size_t>(limit) + 1),
        std::vector<mint>(static_cast<std::size_t>(limit) + 1)
    };
    for (int sum = 0; sum <= limit; ++sum) {
        result.even[sum] = (positive[sum] + negative[sum]) * inverse_two;
        result.odd[sum] = (positive[sum] - negative[sum]) * inverse_two;
    }
    return result;
}

}
