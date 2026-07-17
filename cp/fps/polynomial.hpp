#pragma once
#include <bits/stdc++.h>
#include "fps/fps.hpp"
#include "math/matrix.hpp"

namespace poe {

/// O(M(S) log k)。polynomial_product(polynomials, limit): 多項式列の積を返す。
/// limit>=0なら各段階でx^limit未満へ切り詰める。空積は1。
template<int mod>
fps<mod> polynomial_product(std::vector<fps<mod>> polynomials, int limit = -1) {
    using series = fps<mod>;
    if (polynomials.empty()) return series{1}.prefix(limit < 0 ? 1 : limit);
    auto solve = [&](auto&& self, int left, int right) -> series {
        if (right - left == 1) {
            return limit < 0 ? polynomials[left] : polynomials[left].prefix(limit);
        }
        const int middle = (left + right) / 2;
        series result = self(self, left, middle) * self(self, middle, right);
        if (limit >= 0 && result.size_int() > limit) result.resize(limit);
        return result;
    };
    return solve(solve, 0, static_cast<int>(polynomials.size()));
}

/// O(d^3 M(S) log k)。polynomial_matrix_product(matrices,limit): 行列列を先頭から掛ける。
/// 各要素は多項式で、limit>=0なら各段階でx^limit未満へ切り詰める。行列列は空でないこと。
template<int mod>
matrix<fps<mod>> polynomial_matrix_product(
    std::vector<matrix<fps<mod>>> matrices,
    int limit = -1
) {
    using series = fps<mod>;
    assert(!matrices.empty());
    const int dimension = matrices[0].height();
    for (const auto& current : matrices) {
        assert(current.height() == dimension && current.width() == dimension);
    }
    auto multiply = [&](const matrix<series>& left, const matrix<series>& right) {
        matrix<series> result(dimension, dimension);
        for (int i = 0; i < dimension; ++i) {
            for (int k = 0; k < dimension; ++k) {
                for (int j = 0; j < dimension; ++j) {
                    result[i][j] += left[i][k] * right[k][j];
                    if (limit >= 0 && result[i][j].size_int() > limit) {
                        result[i][j].resize(limit);
                    }
                }
            }
        }
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j) result[i][j].shrink();
        }
        return result;
    };
    auto solve = [&](auto&& self, int left, int right) -> matrix<series> {
        if (right - left == 1) {
            if (limit >= 0) {
                for (int row = 0; row < dimension; ++row) {
                    for (int column = 0; column < dimension; ++column) {
                        matrices[left][row][column] =
                            matrices[left][row][column].prefix(limit);
                        matrices[left][row][column].shrink();
                    }
                }
            }
            return std::move(matrices[left]);
        }
        const int middle = (left + right) / 2;
        return multiply(self(self, left, middle), self(self, middle, right));
    };
    return solve(solve, 0, static_cast<int>(matrices.size()));
}

/// O(M(n) log n)。polynomial_divmod(f,g): 多項式除算の商と余りを返す。g!=0。
template<int mod>
std::pair<fps<mod>, fps<mod>> polynomial_divmod(fps<mod> dividend, fps<mod> divisor) {
    using series = fps<mod>;
    dividend.shrink();
    divisor.shrink();
    assert(!divisor.empty());
    if (dividend.size() < divisor.size()) return {series{}, std::move(dividend)};
    const int quotient_size = dividend.size_int() - divisor.size_int() + 1;
    series reversed_dividend(dividend.rbegin(), dividend.rend());
    series reversed_divisor(divisor.rbegin(), divisor.rend());
    series quotient = (reversed_dividend.prefix(quotient_size)
        * reversed_divisor.inv(quotient_size)).prefix(quotient_size);
    std::reverse(quotient.begin(), quotient.end());
    series remainder = dividend - divisor * quotient;
    remainder.resize(static_cast<std::size_t>(divisor.size_int() - 1));
    remainder.shrink();
    quotient.shrink();
    return {std::move(quotient), std::move(remainder)};
}

/// O(M(n) log^2 n)。multipoint_evaluation(f, points): 全pointsでfを評価する。
template<int mod>
std::vector<staticmodint<mod>> multipoint_evaluation(
    const fps<mod>& polynomial,
    const std::vector<staticmodint<mod>>& points
) {
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    const int count = static_cast<int>(points.size());
    if (count == 0) return {};
    int size = 1;
    while (size < count) size <<= 1;
    std::vector<series> tree(static_cast<std::size_t>(size << 1), series{1});
    for (int i = 0; i < count; ++i) tree[size + i] = series{-points[i], 1};
    for (int node = size - 1; node > 0; --node) tree[node] = tree[node << 1] * tree[node << 1 | 1];

    std::vector<mint> answer(count);
    auto solve = [&](auto&& self, int node, series current) -> void {
        if (node >= size) {
            const int index = node - size;
            if (index < count) answer[index] = current.empty() ? mint{} : current[0];
            return;
        }
        self(self, node << 1, polynomial_divmod(current, tree[node << 1]).second);
        self(self, node << 1 | 1, polynomial_divmod(current, tree[node << 1 | 1]).second);
    };
    solve(solve, 1, polynomial_divmod(polynomial, tree[1]).second);
    return answer;
}

/// O(M(n) log^2 n)。polynomial_interpolation(points, values): 指定点を通る次数n未満の多項式を返す。
/// pointsは相異なること。
template<int mod>
fps<mod> polynomial_interpolation(
    const std::vector<staticmodint<mod>>& points,
    const std::vector<staticmodint<mod>>& values
) {
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    const int count = static_cast<int>(points.size());
    assert(values.size() == points.size());
    if (count == 0) return {};
    int size = 1;
    while (size < count) size <<= 1;
    std::vector<series> tree(static_cast<std::size_t>(size << 1), series{1});
    for (int i = 0; i < count; ++i) tree[size + i] = series{-points[i], 1};
    for (int node = size - 1; node > 0; --node) tree[node] = tree[node << 1] * tree[node << 1 | 1];

    const auto derivative_values = multipoint_evaluation(tree[1].diff(), points);
    std::vector<series> interpolation(static_cast<std::size_t>(size << 1));
    for (int i = 0; i < count; ++i) {
        assert(derivative_values[i] != mint{});
        interpolation[size + i] = series{values[i] / derivative_values[i]};
    }
    for (int node = size - 1; node > 0; --node) {
        interpolation[node] = interpolation[node << 1] * tree[node << 1 | 1]
            + interpolation[node << 1 | 1] * tree[node << 1];
    }
    interpolation[1].resize(count);
    return interpolation[1].shrink();
}

/// O(M(n))。taylor_shift(f,c): f(x+c)を返す。f.size()<mod。
template<int mod>
fps<mod> taylor_shift(const fps<mod>& polynomial, staticmodint<mod> shift) {
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    const int size = polynomial.size_int();
    assert(size < mod);
    if (size == 0) return {};
    std::vector<mint> factorial(size, 1), inverse_factorial(size, 1);
    for (int i = 1; i < size; ++i) factorial[i] = factorial[i - 1] * i;
    inverse_factorial[size - 1] = factorial[size - 1].inv();
    for (int i = size - 1; i > 0; --i) inverse_factorial[i - 1] = inverse_factorial[i] * i;
    series left(size), right(size);
    mint power = 1;
    for (int i = 0; i < size; ++i) {
        left[size - 1 - i] = polynomial[i] * factorial[i];
        right[i] = power * inverse_factorial[i];
        power *= shift;
    }
    series product = left * right;
    series result(size);
    for (int i = 0; i < size; ++i) result[i] = product[size - 1 - i] * inverse_factorial[i];
    return result;
}

}
