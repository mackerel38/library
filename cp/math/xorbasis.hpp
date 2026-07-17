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

}
