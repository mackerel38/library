#pragma once
#include <bits/stdc++.h>
#include "math/gaussian.hpp"
#include "math/modint.hpp"

namespace poe {

/// minimum_binary_perfect_matching_weightへ渡す重み0または1の無向辺。
struct binarymatchingedge {
    int first;
    int second;
    int weight;
};

namespace detail {

template <class T>
T pfaffian(matrix<T> values) {
    assert(values.height() == values.width());
    const int size = values.height();
    if (size & 1) return T{};
    T result = 1;
    for (int first = 0; first < size; first += 2) {
        int second = first + 1;
        while (second < size && values[first][second] == T{}) ++second;
        if (second == size) return T{};
        if (second != first + 1) {
            for (int column = 0; column < size; ++column) {
                std::swap(values[first + 1][column], values[second][column]);
            }
            for (int row = 0; row < size; ++row) {
                std::swap(values[row][first + 1], values[row][second]);
            }
            result = -result;
        }
        const T pivot = values[first][first + 1];
        result *= pivot;
        const T inverse = T{1} / pivot;
        for (int row = first + 2; row < size; ++row) {
            for (int column = row + 1; column < size; ++column) {
                values[row][column] -=
                    (values[first][row] * values[first + 1][column] -
                     values[first][column] * values[first + 1][row]) * inverse;
                values[column][row] = -values[row][column];
            }
        }
    }
    return result;
}

template <class Mint>
std::vector<Mint> interpolate_consecutive_coefficients(
    const std::vector<Mint>& values) {
    const int size = static_cast<int>(values.size());
    std::vector<Mint> product{Mint{1}};
    for (int root = 0; root < size; ++root) {
        product.push_back(Mint{});
        for (int degree = root; degree >= 0; --degree) {
            product[degree + 1] += product[degree];
            product[degree] *= Mint{-root};
        }
    }
    std::vector<Mint> result(size);
    Mint factorial = 1;
    for (int value = 1; value < size; ++value) factorial *= Mint{value};
    Mint denominator = factorial;
    if ((size - 1) & 1) denominator = -denominator;
    for (int point = 0; point < size; ++point) {
        std::vector<Mint> quotient(size);
        quotient[size - 1] = product[size];
        for (int degree = size - 2; degree >= 0; --degree) {
            quotient[degree] = product[degree + 1] + Mint{point} * quotient[degree + 1];
        }
        const Mint scale = values[point] / denominator;
        for (int degree = 0; degree < size; ++degree) {
            result[degree] += scale * quotient[degree];
        }
        if (point + 1 < size) {
            denominator *= Mint{point + 1};
            denominator /= Mint{point + 1 - size};
        }
    }
    return result;
}

}

/// O(n^4)。重み0/1の一般無向グラフで完全matchingの最小重みを高確率で返す。存在しなければnullopt。
inline std::optional<int> minimum_binary_perfect_matching_weight(
    int vertex_count, const std::vector<binarymatchingedge>& edges) {
    assert(vertex_count >= 0);
    if (vertex_count & 1) return std::nullopt;
    if (vertex_count == 0) return 0;
    using mint = staticmodint<998244353>;
    struct randomizededge {
        int first;
        int second;
        int weight;
        mint coefficient;
    };
    std::mt19937 random(0x4122025U);
    std::vector<randomizededge> randomized;
    randomized.reserve(edges.size());
    for (const auto [first, second, weight] : edges) {
        assert(0 <= first && first < vertex_count);
        assert(0 <= second && second < vertex_count && first != second);
        assert(weight == 0 || weight == 1);
        randomized.push_back({
            std::min(first, second), std::max(first, second), weight,
            mint{1 + random() % (mint::mod() - 1)}
        });
    }

    const int maximum_weight = vertex_count / 2;
    std::vector<mint> values(maximum_weight + 1);
    for (int point = 0; point <= maximum_weight; ++point) {
        matrix<mint> tutte(vertex_count, vertex_count);
        const mint variable = point;
        for (const auto& edge : randomized) {
            const mint value = edge.coefficient *
                (edge.weight == 0 ? mint{1} : variable);
            tutte[edge.first][edge.second] += value;
            tutte[edge.second][edge.first] -= value;
        }
        values[point] = detail::pfaffian(std::move(tutte));
    }
    const auto coefficient =
        detail::interpolate_consecutive_coefficients(values);
    for (int degree = 0; degree <= maximum_weight; ++degree) {
        if (coefficient[degree] != mint{}) {
            return degree;
        }
    }
    return std::nullopt;
}

}
