#pragma once
#include <bits/stdc++.h>

namespace poe {

/// xorbasis<UInt>: XOR線形基底。add・contains・min・max・kthを各O(bit)で行う。
template<std::unsigned_integral UInt = unsigned long long>
struct xorbasis {
    static constexpr int bits = std::numeric_limits<UInt>::digits;
    static_assert(bits <= 64, "xorbasis::kth uses a 64-bit index");

    /// O(bit)。basis.add(x): xが独立なら追加してtrue、従属ならfalseを返す。
    bool add(UInt value) {
        for (int bit = bits - 1; bit >= 0; --bit) {
            if (((value >> bit) & 1) == 0) continue;
            if (basis_[bit] != 0) value ^= basis_[bit];
            else {
                basis_[bit] = value;
                ++rank_;
                return true;
            }
        }
        return false;
    }

    /// O(bit)。basis.contains(x): 基底の部分集合XORでxを作れるか返す。
    bool contains(UInt value) const {
        for (int bit = bits - 1; bit >= 0; --bit) {
            if ((value >> bit) & 1) value ^= basis_[bit];
        }
        return value == 0;
    }

    /// O(1)。basis.rank(): 基底のrankを返す。
    int rank() const noexcept { return rank_; }

    /// O(bit)。basis.max(x): xと部分集合XORのXORとして作れる最大値を返す。
    UInt max(UInt value = 0) const {
        for (int bit = bits - 1; bit >= 0; --bit) value = std::max(value, value ^ basis_[bit]);
        return value;
    }

    /// O(bit)。basis.min(x): xと部分集合XORのXORとして作れる最小値を返す。
    UInt min(UInt value = 0) const {
        for (int bit = bits - 1; bit >= 0; --bit) value = std::min(value, value ^ basis_[bit]);
        return value;
    }

    /// O(bit^2)。basis.vectors(): 縮約した基底をpivot昇順で返す。
    std::vector<UInt> vectors() const {
        auto reduced = basis_;
        for (int high = 0; high < bits; ++high) {
            if (reduced[high] == 0) continue;
            for (int low = 0; low < high; ++low) {
                if ((reduced[high] >> low) & 1) reduced[high] ^= reduced[low];
            }
        }
        std::vector<UInt> result;
        for (UInt value : reduced) if (value != 0) result.push_back(value);
        return result;
    }

    /// O(bit^2)。basis.kth(k): 作れる値を昇順に並べた0-indexed k番目を返す。
    UInt kth(unsigned long long index) const {
        assert(rank_ == 64 || index < (1ULL << rank_));
        const auto reduced = vectors();
        UInt result = 0;
        for (int i = 0; i < static_cast<int>(reduced.size()); ++i) {
            if ((index >> i) & 1) result ^= reduced[i];
        }
        return result;
    }

    /// O(bit^2)。basis.merge(other): otherの張る空間を追加する。
    void merge(const xorbasis& other) {
        for (UInt value : other.basis_) if (value != 0) add(value);
    }

private:
    std::array<UInt, bits> basis_{};
    int rank_ = 0;
};

/// O(n bits+bits^3)時間・O(bits^2)領域。非空部分集合のfirst XOR<=upperでsecond XORを最大化し、存在しなければnullopt。
template<std::unsigned_integral UInt, int Bits = std::numeric_limits<UInt>::digits / 2>
std::optional<UInt> maximum_paired_xor_under(
    const std::vector<std::pair<UInt, UInt>>& values,
    UInt upper
) {
    static_assert(1 <= Bits && 2 * Bits <= 64 && Bits <= std::numeric_limits<UInt>::digits);
    if constexpr (Bits < std::numeric_limits<UInt>::digits) {
        assert(upper < (UInt{1} << Bits));
        for (const auto& [first, second] : values) {
            assert(first < (UInt{1} << Bits) && second < (UInt{1} << Bits));
        }
    }

    xorbasis<unsigned long long> combined;
    for (const auto& [first, second] : values) {
        combined.add((static_cast<unsigned long long>(first) << Bits) | second);
    }
    const auto basis = combined.vectors();
    const int rank = static_cast<int>(basis.size());
    const bool dependent = static_cast<int>(values.size()) > rank;

    const auto optimize = [&](int lowest_fixed_bit, UInt fixed) -> std::optional<UInt> {
        std::array<unsigned long long, 64> equation{};
        std::array<unsigned char, 64> right{};
        bool inconsistent = false;
        for (int bit = Bits - 1; bit >= lowest_fixed_bit; --bit) {
            unsigned long long mask = 0;
            for (int variable = 0; variable < rank; ++variable) {
                if ((basis[variable] >> (Bits + bit)) & 1ULL) mask |= 1ULL << variable;
            }
            unsigned char value = (fixed >> bit) & 1;
            while (mask != 0) {
                const int pivot = std::bit_width(mask) - 1;
                if (equation[pivot] == 0) {
                    equation[pivot] = mask;
                    right[pivot] = value;
                    break;
                }
                mask ^= equation[pivot];
                value ^= right[pivot];
            }
            if (mask == 0 && value != 0) inconsistent = true;
        }
        if (inconsistent) return std::nullopt;

        unsigned long long particular = 0;
        for (int pivot = 0; pivot < rank; ++pivot) {
            if (equation[pivot] == 0) continue;
            const int parity = std::popcount(equation[pivot] & particular) & 1;
            if (parity != right[pivot]) particular |= 1ULL << pivot;
        }
        const auto second_value = [&](unsigned long long coefficient) {
            UInt result = 0;
            for (int variable = 0; variable < rank; ++variable) {
                if ((coefficient >> variable) & 1ULL) {
                    result ^= static_cast<UInt>(basis[variable]);
                }
            }
            if constexpr (Bits < std::numeric_limits<UInt>::digits) {
                result &= (UInt{1} << Bits) - 1;
            }
            return result;
        };

        xorbasis<UInt> second_basis;
        bool has_free = false;
        for (int free = 0; free < rank; ++free) {
            if (equation[free] != 0) continue;
            has_free = true;
            unsigned long long coefficient = 1ULL << free;
            for (int pivot = 0; pivot < rank; ++pivot) {
                if (equation[pivot] == 0) continue;
                if (std::popcount(equation[pivot] & coefficient) & 1) {
                    coefficient |= 1ULL << pivot;
                }
            }
            second_basis.add(second_value(coefficient));
        }
        const UInt best = second_basis.max(second_value(particular));
        if (best != 0 || particular != 0 || has_free || dependent) return best;
        return std::nullopt;
    };

    std::optional<UInt> answer = optimize(0, upper);
    for (int bit = Bits - 1; bit >= 0; --bit) {
        if (((upper >> bit) & 1) == 0) continue;
        const UInt fixed = upper ^ (UInt{1} << bit);
        const auto candidate = optimize(bit, fixed);
        if (candidate && (!answer || *answer < *candidate)) answer = candidate;
    }
    return answer;
}

}
