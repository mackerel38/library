#pragma once
#include <bits/stdc++.h>
#include "fps/fps.hpp"

namespace poe {

namespace detail {

template<int mod>
struct composition_ntt_table {
    using mint = staticmodint<mod>;
    std::array<mint, 30> root{}, inverse_root{}, rate{}, inverse_rate{};

    composition_ntt_table() {
        const int rank = std::countr_zero(static_cast<unsigned int>(mod - 1));
        root[rank] = mint{primitive_root(mod)}.pow((mod - 1) >> rank);
        inverse_root[rank] = root[rank].inv();
        for (int i = rank; i > 0; --i) {
            root[i - 1] = root[i] * root[i];
            inverse_root[i - 1] = inverse_root[i] * inverse_root[i];
        }
        mint product = 1, inverse_product = 1;
        for (int i = 0; i <= rank - 2; ++i) {
            rate[i] = root[i + 2] * product;
            inverse_rate[i] = inverse_root[i + 2] * inverse_product;
            product *= inverse_root[i + 2];
            inverse_product *= root[i + 2];
        }
    }
};

template<int mod>
const composition_ntt_table<mod>& get_composition_ntt_table() {
    static const composition_ntt_table<mod> table;
    return table;
}

template<int mod>
void composition_ntt(std::vector<staticmodint<mod>>& values, bool inverse) {
    using mint = staticmodint<mod>;
    const int size = static_cast<int>(values.size());
    const int height = std::countr_zero(static_cast<unsigned int>(size));
    const auto& table = get_composition_ntt_table<mod>();
    if (!inverse) {
        for (int level = 0; level < height; ++level) {
            const int half = 1 << (height - level - 1);
            mint rotation = 1;
            for (int block = 0; block < (1 << level); ++block) {
                const int offset = block << (height - level);
                for (int i = 0; i < half; ++i) {
                    const mint left = values[offset + i];
                    const mint right = values[offset + half + i] * rotation;
                    values[offset + i] = left + right;
                    values[offset + half + i] = left - right;
                }
                rotation *= table.rate[std::countr_zero(~static_cast<unsigned int>(block))];
            }
        }
    } else {
        const mint inverse_size = mint{size}.inv();
        for (mint& value : values) value *= inverse_size;
        for (int level = height; level > 0; --level) {
            const int half = 1 << (height - level);
            mint rotation = 1;
            for (int block = 0; block < (1 << (level - 1)); ++block) {
                const int offset = block << (height - level + 1);
                for (int i = 0; i < half; ++i) {
                    const mint left = values[offset + i];
                    const mint right = values[offset + half + i];
                    values[offset + i] = left + right;
                    values[offset + half + i] = (left - right) * rotation;
                }
                rotation *= table.inverse_rate[std::countr_zero(~static_cast<unsigned int>(block))];
            }
        }
    }
}

template<int mod>
void composition_transposed_ntt(std::vector<staticmodint<mod>>& values, bool inverse) {
    using mint = staticmodint<mod>;
    const int size = static_cast<int>(values.size());
    const int height = std::countr_zero(static_cast<unsigned int>(size));
    const auto& table = get_composition_ntt_table<mod>();
    if (!inverse) {
        for (int level = height; level > 0; --level) {
            const int half = 1 << (height - level);
            mint rotation = 1;
            for (int block = 0; block < (1 << (level - 1)); ++block) {
                const int offset = block << (height - level + 1);
                for (int i = 0; i < half; ++i) {
                    const mint left = values[offset + i];
                    const mint right = values[offset + half + i];
                    values[offset + i] = left + right;
                    values[offset + half + i] = (left - right) * rotation;
                }
                rotation *= table.rate[std::countr_zero(~static_cast<unsigned int>(block))];
            }
        }
    } else {
        const mint inverse_size = mint{size}.inv();
        for (mint& value : values) value *= inverse_size;
        for (int level = 0; level < height; ++level) {
            const int half = 1 << (height - level - 1);
            mint rotation = 1;
            for (int block = 0; block < (1 << level); ++block) {
                const int offset = block << (height - level);
                for (int i = 0; i < half; ++i) {
                    const mint left = values[offset + i];
                    const mint right = values[offset + half + i] * rotation;
                    values[offset + i] = left + right;
                    values[offset + half + i] = left - right;
                }
                rotation *= table.inverse_rate[std::countr_zero(~static_cast<unsigned int>(block))];
            }
        }
    }
}

/// O(n log^2 n)。定数項0、同じ2冪長のFPSを高速合成する。2n|(mod-1)。
template<int mod>
fps<mod> compose_zero_fast(fps<mod> outer, fps<mod> inner) {
    using series = fps<mod>;
    using mint = staticmodint<mod>;
    const int length = outer.size_int();
    assert(length == inner.size_int() && std::has_single_bit(static_cast<unsigned int>(length)));
    assert((mod - 1) % (2 * length) == 0 && (inner.empty() || inner[0] == mint{}));
    if (length == 1) return series{outer[0]};

    // butterfly順に並んだ2n乗根。
    const int rank = std::countr_zero(static_cast<unsigned int>(mod - 1));
    const mint primitive_power_two_root = mint{poe::detail::primitive_root(mod)}.pow((mod - 1) >> rank);
    const mint half_root = primitive_power_two_root.inv().pow((1 << rank) / (2 * length));
    std::vector<mint> roots(length);
    std::vector<int> bit_reverse(length);
    const int height = std::countr_zero(static_cast<unsigned int>(length));
    for (int i = 0; i < length; ++i) {
        bit_reverse[i] = (bit_reverse[i >> 1] >> 1) + ((i & 1) << (height - 1));
    }
    mint power = 1;
    for (int index : bit_reverse) {
        roots[index] = power;
        power *= half_root;
    }

    auto recurse = [&](auto&& self, int x_size, int y_size, std::vector<mint>& denominator)
        -> std::vector<mint> {
        if (x_size == 1) {
            std::reverse(outer.begin(), outer.end());
            composition_transposed_ntt<mod>(outer, true);
            const mint inverse_size = mint{y_size}.inv();
            for (mint& value : outer) value *= inverse_size;
            std::vector<mint> numerator(4 * y_size);
            for (int y = 0; y < y_size; ++y) numerator[2 * y] = outer[y];
            return numerator;
        }

        auto extend_y = [&](std::vector<mint>& values, int left, int right, bool transpose) {
            const mint shift = roots[y_size / 2].inv();
            std::vector<mint> column(y_size);
            if (!transpose) {
                for (int x = left; x < right; ++x) {
                    for (int y = 0; y < y_size; ++y) column[y] = values[2 * x_size * y + x];
                    composition_ntt<mod>(column, true);
                    mint power = 1;
                    for (int y = 1; y < y_size; ++y) {
                        power *= shift;
                        column[y] *= power;
                    }
                    composition_ntt<mod>(column, false);
                    for (int y = 0; y < y_size; ++y) {
                        values[2 * x_size * (y_size + y) + x] = column[y];
                    }
                }
            } else {
                for (int x = left; x < right; ++x) {
                    for (int y = 0; y < y_size; ++y) {
                        column[y] = values[2 * x_size * (y_size + y) + x];
                    }
                    composition_transposed_ntt<mod>(column, false);
                    mint power = 1;
                    for (int y = 1; y < y_size; ++y) {
                        power *= shift;
                        column[y] *= power;
                    }
                    composition_transposed_ntt<mod>(column, true);
                    for (int y = 0; y < y_size; ++y) values[2 * x_size * y + x] += column[y];
                }
            }
        };

        auto transform_x = [&](std::vector<mint>& values, int left, int right, bool transpose) {
            std::vector<mint> row(2 * x_size);
            for (int y = left; y < right; ++y) {
                std::copy_n(values.begin() + 2 * x_size * y, 2 * x_size, row.begin());
                if (transpose) composition_transposed_ntt<mod>(row, false);
                else composition_ntt<mod>(row, false);
                std::copy(row.begin(), row.end(), values.begin() + 2 * x_size * y);
            }
        };

        if (x_size <= y_size) {
            extend_y(denominator, 1, x_size, false);
            transform_x(denominator, 0, 2 * y_size, false);
        } else {
            transform_x(denominator, 0, y_size, false);
            extend_y(denominator, 0, 2 * x_size, false);
        }

        for (int i = 0; i < 2 * x_size * y_size; ++i) denominator[i] += 1;
        for (int i = 2 * x_size * y_size; i < 4 * x_size * y_size; ++i) denominator[i] -= 1;

        std::vector<mint> next_denominator(4 * x_size * y_size);
        std::vector<mint> frequency(2 * x_size), even(x_size);
        for (int y = 0; y < 2 * y_size; ++y) {
            std::copy_n(denominator.begin() + 2 * x_size * y, 2 * x_size, frequency.begin());
            for (int x = 0; x < x_size; ++x) even[x] = frequency[2 * x] * frequency[2 * x + 1];
            composition_ntt<mod>(even, true);
            std::copy_n(even.begin(), x_size / 2, next_denominator.begin() + x_size * y);
        }
        for (int y = 0; y < 4 * y_size; ++y) next_denominator[x_size * y] = 0;

        std::vector<mint> numerator = self(self, x_size / 2, 2 * y_size, next_denominator);
        std::vector<mint> half(x_size), lifted(2 * x_size);
        for (int y = 2 * y_size - 1; y >= 0; --y) {
            std::copy_n(numerator.begin() + x_size * y, x_size / 2, half.begin());
            std::fill(half.begin() + x_size / 2, half.end(), mint{});
            std::copy_n(denominator.begin() + 2 * x_size * y, 2 * x_size, frequency.begin());
            composition_transposed_ntt<mod>(half, true);
            for (int x = 0; x < x_size; ++x) {
                half[x] *= roots[x];
                lifted[2 * x] = frequency[2 * x + 1] * half[x];
                lifted[2 * x + 1] = -frequency[2 * x] * half[x];
            }
            std::copy(lifted.begin(), lifted.end(), numerator.begin() + 2 * x_size * y);
        }

        if (x_size <= y_size) {
            transform_x(numerator, 0, 2 * y_size, true);
            extend_y(numerator, 0, x_size, true);
        } else {
            extend_y(numerator, 0, 2 * x_size, true);
            transform_x(numerator, 0, y_size, true);
        }
        return numerator;
    };

    std::vector<mint> denominator(4 * length);
    for (int i = 0; i < length; ++i) denominator[i] = -inner[i];
    std::vector<mint> numerator = recurse(recurse, length, 1, denominator);
    numerator.resize(length);
    std::reverse(numerator.begin(), numerator.end());
    return series(std::move(numerator));
}

}

/// NTT可能ならO(n log^2 n)、それ以外はO(n^2+sqrt(n)M(n))。compose(f,g,n): f(g(x)) mod x^nを返す。g[0]=0。
template<int mod>
fps<mod> compose(const fps<mod>& outer, const fps<mod>& inner, int size = -1) {
    using series = fps<mod>;
    using mint = staticmodint<mod>;
    if (size < 0) size = std::max(outer.size_int(), inner.size_int());
    assert(size >= 0 && (inner.empty() || inner[0] == mint{}));
    if (size == 0) return {};
    int transform_size = 1;
    while (transform_size < size) transform_size <<= 1;
    if (transform_size >= 64 && (mod - 1) % (2 * transform_size) == 0) {
        return detail::compose_zero_fast<mod>(outer.prefix(transform_size), inner.prefix(transform_size)).prefix(size);
    }
    int block = 1;
    while (block * block < size) ++block;
    std::vector<series> baby(block + 1), giant(block + 1);
    baby[0] = series{1};
    for (int i = 1; i <= block; ++i) baby[i] = (baby[i - 1] * inner).prefix(size);
    giant[0] = series{1};
    for (int i = 1; i <= block; ++i) giant[i] = (giant[i - 1] * baby[block]).prefix(size);
    series result(size);
    for (int high = 0; high <= block; ++high) {
        series low(size);
        for (int offset = 0; offset < block; ++offset) {
            const int index = high * block + offset;
            if (index >= outer.size_int()) break;
            for (int degree = 0; degree < baby[offset].size_int(); ++degree) {
                low[degree] += outer[index] * baby[offset][degree];
            }
        }
        result += (low * giant[high]).prefix(size);
        result.resize(size);
    }
    return result;
}

/// O(log k)回のcompose。functional_power(f,k,n): fをk回自己合成する。恒等写像はx。
template<int mod>
fps<mod> functional_power(fps<mod> function, long long exponent, int size = -1) {
    using series = fps<mod>;
    if (size < 0) size = function.size_int();
    assert(exponent >= 0 && size >= 0 && (function.empty() || function[0] == staticmodint<mod>{}));
    series result(size);
    if (size >= 2) result[1] = 1;
    while (exponent > 0) {
        if (exponent & 1) result = compose<mod>(result, function, size);
        exponent >>= 1;
        if (exponent) function = compose<mod>(function, function, size);
    }
    return result;
}

}
