#pragma once
#include <bits/stdc++.h>
#include "fps/polynomial.hpp"

namespace poe {

/// divisible_group_countsの結果。ordinaryは要素を区別せず、labeledは各要素を区別する。
template<class T>
struct groupdistributioncounts {
    T ordinary;
    T labeled;
};

/// adjacent_distinct_wordsの結果。countとlength_sumを重み和ごとのFPSで持つ。
template<int mod>
struct adjacentdistinctwordcounts {
    fps<mod> count;
    fps<mod> length_sum;
};

/// O(kn+M(n)log k)。n要素を各groupの大きさがperiodの倍数となるよう分配する通り数を返す。
template<int mod>
groupdistributioncounts<staticmodint<mod>> divisible_group_counts(
    int element_count,
    const std::vector<int>& periods
) {
    assert(0 <= element_count && element_count < mod);
    for (const int period : periods) assert(period > 0);
    using mint = staticmodint<mod>;
    using series = fps<mod>;

    std::vector<mint> factorial(static_cast<std::size_t>(element_count) + 1, 1);
    std::vector<mint> inverse_factorial(static_cast<std::size_t>(element_count) + 1, 1);
    for (int value = 1; value <= element_count; ++value) {
        factorial[value] = factorial[value - 1] * value;
    }
    inverse_factorial[element_count] = factorial[element_count].inv();
    for (int value = element_count; value >= 1; --value) {
        inverse_factorial[value - 1] = inverse_factorial[value] * value;
    }

    std::vector<series> ordinary_factors;
    std::vector<series> labeled_factors;
    ordinary_factors.reserve(periods.size());
    labeled_factors.reserve(periods.size());
    for (const int period : periods) {
        series ordinary(static_cast<std::size_t>(element_count) + 1);
        series labeled(static_cast<std::size_t>(element_count) + 1);
        for (int size = 0; size <= element_count; size += period) {
            ordinary[size] = 1;
            labeled[size] = inverse_factorial[size];
        }
        ordinary_factors.push_back(std::move(ordinary));
        labeled_factors.push_back(std::move(labeled));
    }
    const auto ordinary = polynomial_product<mod>(std::move(ordinary_factors), element_count + 1);
    const auto labeled = polynomial_product<mod>(std::move(labeled_factors), element_count + 1);
    return {
        ordinary.coeff(element_count),
        labeled.coeff(element_count) * factorial[element_count]
    };
}

/// O(M(n)log n)。各頂点vにdegree_weight[deg(v)]を掛けたn頂点ラベル付き木の重み総和を返す。
template<int mod>
staticmodint<mod> labeled_tree_degree_weight_sum(
    int vertex_count,
    const std::vector<staticmodint<mod>>& degree_weight
) {
    assert(2 <= vertex_count && vertex_count < mod);
    assert(static_cast<int>(degree_weight.size()) >= vertex_count);
    using mint = staticmodint<mod>;
    using series = fps<mod>;

    const int code_length = vertex_count - 2;
    std::vector<mint> inverse_factorial(static_cast<std::size_t>(code_length) + 1, 1);
    mint factorial = 1;
    for (int count = 1; count <= code_length; ++count) factorial *= count;
    inverse_factorial[code_length] = factorial.inv();
    for (int count = code_length; count >= 1; --count) {
        inverse_factorial[count - 1] = inverse_factorial[count] * count;
    }

    series choice(static_cast<std::size_t>(code_length) + 1);
    for (int appearances = 0; appearances <= code_length; ++appearances) {
        choice[appearances] = degree_weight[appearances + 1] * inverse_factorial[appearances];
    }
    return choice.pow(vertex_count, code_length + 1)[code_length] * factorial;
}

/// O(n+M(n)log n)。全頂点の次数がallowed_degreesのいずれかであるn頂点ラベル付き木を数える。
template<int mod>
staticmodint<mod> count_labeled_trees_with_degrees(
    int vertex_count,
    const std::vector<int>& allowed_degrees
) {
    assert(2 <= vertex_count && vertex_count < mod);
    std::vector<staticmodint<mod>> degree_weight(vertex_count);
    for (const int degree : allowed_degrees) {
        assert(1 <= degree && degree < vertex_count);
        degree_weight[degree] = 1;
    }
    return labeled_tree_degree_weight_sum<mod>(vertex_count, degree_weight);
}

/// O(sum(max_sum/weight)+M(n)log n)。同じ記号が隣接しないwordの個数と長さ総和を返す。
template<int mod>
adjacentdistinctwordcounts<mod> adjacent_distinct_words(
    int maximum_sum,
    const std::vector<int>& symbol_weights
) {
    assert(maximum_sum >= 0);
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    series block(static_cast<std::size_t>(maximum_sum) + 1);
    series marked_block(static_cast<std::size_t>(maximum_sum) + 1);
    for (const int weight : symbol_weights) {
        assert(weight > 0);
        for (int count = 1, sum = weight; sum <= maximum_sum; ++count, sum += weight) {
            const mint sign = count & 1 ? mint{1} : mint{-1};
            block[sum] += sign;
            marked_block[sum] += sign * count;
        }
    }
    series denominator = -block;
    denominator[0] += 1;
    series count = denominator.inv(maximum_sum + 1);
    series length_sum = (marked_block * count * count).prefix(maximum_sum + 1);
    return {std::move(count), std::move(length_sum)};
}

}
