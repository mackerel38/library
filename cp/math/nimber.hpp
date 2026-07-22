#pragma once
#include <bits/stdc++.h>

namespace poe {

namespace detail {

inline std::uint64_t nim_product_recursive(
    std::uint64_t left, std::uint64_t right, int bits,
    const std::array<std::array<std::uint64_t, 256>, 256>* small
) {
    if (left < right) std::swap(left, right);
    if (right <= 1) return left * right;
    if (bits <= 8 && small != nullptr) {
        return (*small)[static_cast<std::size_t>(left)]
                       [static_cast<std::size_t>(right)];
    }
    const int half = bits / 2;
    const std::uint64_t mask = (std::uint64_t{1} << half) - 1;
    const std::uint64_t left_high = left >> half;
    const std::uint64_t left_low = left & mask;
    const std::uint64_t right_high = right >> half;
    const std::uint64_t right_low = right & mask;
    const std::uint64_t high = nim_product_recursive(
        left_high, right_high, half, small);
    const std::uint64_t low = nim_product_recursive(
        left_low, right_low, half, small);
    const std::uint64_t mixed = nim_product_recursive(
        left_high ^ left_low, right_high ^ right_low, half, small);
    return nim_product_recursive(
               high, std::uint64_t{1} << (half - 1), half, small) ^
           low ^ ((low ^ mixed) << half);
}

inline const std::array<std::array<std::uint64_t, 256>, 256>&
nim_product_table() {
    static const auto table = [] {
        std::array<std::array<std::uint64_t, 256>, 256> result{};
        for (int left = 0; left < 256; ++left) {
            for (int right = 0; right < 256; ++right) {
                result[left][right] = nim_product_recursive(
                    static_cast<std::uint64_t>(left),
                    static_cast<std::uint64_t>(right), 8, nullptr);
            }
        }
        return result;
    }();
    return table;
}

}

/// O(1)。64-bit nimberのNim積left tensor rightを返す。
inline std::uint64_t nim_product(std::uint64_t left, std::uint64_t right) {
    return detail::nim_product_recursive(
        left, right, 64, &detail::nim_product_table());
}

/// XORを加法、Nim積を乗法とする64-bit nimber体の要素。
struct nimber {
    std::uint64_t value = 0;

    /// O(1)。整数valueを同じbit表現のnimberにする。
    constexpr nimber(std::uint64_t value = 0) : value(value) {}

    /// O(1)。加法はbitwise XOR。
    nimber& operator+=(nimber other) {
        value ^= other.value;
        return *this;
    }

    /// O(1)。乗法はNim積。
    nimber& operator*=(nimber other) {
        value = nim_product(value, other.value);
        return *this;
    }

    friend nimber operator+(nimber left, nimber right) { return left += right; }
    friend nimber operator-(nimber left, nimber right) { return left += right; }
    friend nimber operator*(nimber left, nimber right) { return left *= right; }
    friend bool operator==(nimber, nimber) = default;
};

}
