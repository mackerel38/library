#pragma once
#include <bits/stdc++.h>
#include "math/nimber.hpp"

namespace poe {

/// 要素ごとのXORと両立するnimber Rolling Hash: xorrollinghash hash(values)。
struct xorrollinghash {
    using value_type = std::uint64_t;

    /// O(n M)。64-bit非負整数列valuesのhashを構築する。MはNim積の計算量。
    explicit xorrollinghash(
        const std::vector<std::uint64_t>& values,
        std::uint64_t base = default_base()
    ) : values_(values), base_(base), prefix_(values.size() + 1),
        power_(values.size() + 1, 1) {
        assert(base_ != 0 && base_ != 1);
        for (int index = 0; index < size(); ++index) {
            power_[index + 1] = nim_product(power_[index], base_);
            prefix_[index + 1] =
                nim_product(prefix_[index], base_) ^ values_[index];
        }
    }

    /// O(1)。列長を返す。
    int size() const noexcept { return static_cast<int>(values_.size()); }

    /// O(1)。元の列のindex番目を返す。
    std::uint64_t operator[](int index) const {
        assert(0 <= index && index < size());
        return values_[index];
    }

    /// O(M)。半開区間[left,right)のnimber多項式hashを返す。
    value_type hash(int left, int right) const {
        assert(0 <= left && left <= right && right <= size());
        return prefix_[right] ^
               nim_product(prefix_[left], power_[right - left]);
    }

    /// O(M)。等長二部分列を要素ごとにXORした列のhashを返す。
    value_type xor_hash(
        int first_left, int first_right, int second_left, int second_right
    ) const {
        return xor_hash(first_left, first_right, *this,
                        second_left, second_right);
    }

    /// O(M)。この列とotherの等長部分列を要素ごとにXORしたhashを返す。
    value_type xor_hash(
        int first_left, int first_right, const xorrollinghash& other,
        int second_left, int second_right
    ) const {
        assert(base_ == other.base_);
        assert(first_right - first_left == second_right - second_left);
        return hash(first_left, first_right) ^
               other.hash(second_left, second_right);
    }

    /// O(M log length)。二部分列の要素ごとのXORとtarget部分列のLCP長を返す。
    int xor_lcp(
        int first_left, int second_left, int xor_length,
        int target_left, int target_right
    ) const {
        return xor_lcp(first_left, first_left + xor_length, *this,
                       second_left, second_left + xor_length, *this,
                       target_left, target_right);
    }

    /// O(M log length)。この列とotherのXOR列とtarget部分列のLCP長を返す。
    int xor_lcp(
        int first_left, int first_right,
        const xorrollinghash& other, int second_left, int second_right,
        const xorrollinghash& target, int target_left, int target_right
    ) const {
        assert(base_ == other.base_ && base_ == target.base_);
        assert(0 <= first_left && first_left <= first_right &&
               first_right <= size());
        assert(0 <= second_left && second_left <= second_right &&
               second_right <= other.size());
        assert(first_right - first_left == second_right - second_left);
        const int xor_length = first_right - first_left;
        assert(0 <= target_left && target_left <= target_right &&
               target_right <= target.size());
        int low = 0;
        int high = std::min(xor_length, target_right - target_left) + 1;
        while (high - low > 1) {
            const int middle = (low + high) / 2;
            if ((hash(first_left, first_left + middle) ^
                 other.hash(second_left, second_left + middle) ^
                 target.hash(target_left, target_left + middle)) == 0) {
                low = middle;
            } else {
                high = middle;
            }
        }
        return low;
    }

    /// O(1)。このhashが使うnimber基数を返す。
    std::uint64_t base() const noexcept { return base_; }

private:
    static std::uint64_t default_base() {
        static const std::uint64_t value = [] {
            std::uint64_t seed = static_cast<std::uint64_t>(
                std::chrono::steady_clock::now().time_since_epoch().count());
            seed ^= reinterpret_cast<std::uintptr_t>(&seed);
            std::mt19937_64 random(seed);
            std::uint64_t result = random();
            while (result <= 1) result = random();
            return result;
        }();
        return value;
    }

    std::vector<std::uint64_t> values_;
    std::uint64_t base_;
    std::vector<std::uint64_t> prefix_;
    std::vector<std::uint64_t> power_;
};

}
