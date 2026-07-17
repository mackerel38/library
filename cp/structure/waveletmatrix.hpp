#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 静的列の値域query: waveletmatrix<int, long long> wm(values); 構築O(n log sigma)、取得O(log sigma)。
template <class T, class Sum = long long>
struct waveletmatrix {
    /// O(n log sigma)。値を座標圧縮して構築する。
    explicit waveletmatrix(const std::vector<T>& values) : values_(values) {
        coordinates_ = values;
        std::sort(coordinates_.begin(), coordinates_.end());
        coordinates_.erase(std::unique(coordinates_.begin(), coordinates_.end()),
                           coordinates_.end());
        const int sigma = static_cast<int>(coordinates_.size());
        levels_ = sigma <= 1 ? 0 : std::bit_width(static_cast<unsigned>(sigma - 1));

        std::vector<int> ranks(size());
        for (int index = 0; index < size(); ++index) {
            ranks[index] = static_cast<int>(
                std::lower_bound(coordinates_.begin(), coordinates_.end(), values[index]) -
                coordinates_.begin());
        }
        std::vector<T> current_values = values;
        one_prefix_.assign(levels_, std::vector<int>(size() + 1));
        zero_sum_.assign(levels_, std::vector<Sum>(size() + 1));
        middle_.resize(levels_);

        for (int level = levels_ - 1; level >= 0; --level) {
            std::vector<int> zero_ranks, one_ranks;
            std::vector<T> zero_values, one_values;
            zero_ranks.reserve(size());
            one_ranks.reserve(size());
            zero_values.reserve(size());
            one_values.reserve(size());
            for (int index = 0; index < size(); ++index) {
                const bool bit = (ranks[index] >> level) & 1;
                one_prefix_[level][index + 1] = one_prefix_[level][index] + bit;
                zero_sum_[level][index + 1] = zero_sum_[level][index];
                if (bit) {
                    one_ranks.push_back(ranks[index]);
                    one_values.push_back(current_values[index]);
                } else {
                    zero_sum_[level][index + 1] +=
                        static_cast<Sum>(current_values[index]);
                    zero_ranks.push_back(ranks[index]);
                    zero_values.push_back(current_values[index]);
                }
            }
            middle_[level] = static_cast<int>(zero_ranks.size());
            ranks.clear();
            ranks.insert(ranks.end(), zero_ranks.begin(), zero_ranks.end());
            ranks.insert(ranks.end(), one_ranks.begin(), one_ranks.end());
            current_values.clear();
            current_values.insert(current_values.end(), zero_values.begin(),
                                  zero_values.end());
            current_values.insert(current_values.end(), one_values.begin(),
                                  one_values.end());
        }

        original_sum_.resize(size() + 1);
        for (int index = 0; index < size(); ++index) {
            original_sum_[index + 1] =
                original_sum_[index] + static_cast<Sum>(values[index]);
        }
    }

    /// O(1)。要素数を返す。
    int size() const noexcept {
        return static_cast<int>(values_.size());
    }

    /// O(1)。wm[index]で元の列の要素を返す。
    const T& operator[](int index) const {
        assert(0 <= index && index < size());
        return values_[index];
    }

    /// O(log sigma)。[left,right)で0-indexedのk番目に小さい値を返す。
    T kth(int left, int right, int k) const {
        assert_range(left, right);
        assert(0 <= k && k < right - left);
        int rank = 0;
        for (int level = levels_ - 1; level >= 0; --level) {
            const int ones_left = one_prefix_[level][left];
            const int ones_right = one_prefix_[level][right];
            const int zeros = (right - left) - (ones_right - ones_left);
            if (k < zeros) {
                left -= ones_left;
                right -= ones_right;
            } else {
                k -= zeros;
                rank |= 1 << level;
                left = middle_[level] + ones_left;
                right = middle_[level] + ones_right;
            }
        }
        return coordinates_[rank];
    }

    /// O(log sigma)。[left,right)にあるupper未満の要素数を返す。
    int count_less(int left, int right, const T& upper) const {
        const int rank = static_cast<int>(
            std::lower_bound(coordinates_.begin(), coordinates_.end(), upper) -
            coordinates_.begin());
        return count_rank_less(left, right, rank);
    }

    /// O(log sigma)。[left,right)にあるupper以下の要素数を返す。
    int count_leq(int left, int right, const T& upper) const {
        const int rank = static_cast<int>(
            std::upper_bound(coordinates_.begin(), coordinates_.end(), upper) -
            coordinates_.begin());
        return count_rank_less(left, right, rank);
    }

    /// O(log sigma)。[left,right)にあるvalueの個数を返す。
    int count(int left, int right, const T& value) const {
        return count_leq(left, right, value) - count_less(left, right, value);
    }

    /// O(log sigma)。[left,right)にある[lower,upper)の要素数を返す。
    int count(int left, int right, const T& lower, const T& upper) const {
        return count_less(left, right, upper) - count_less(left, right, lower);
    }

    /// O(1)。[left,right)の全要素の和を返す。
    Sum sum(int left, int right) const {
        assert_range(left, right);
        return original_sum_[right] - original_sum_[left];
    }

    /// O(log sigma)。[left,right)にあるupper未満の要素の和を返す。
    Sum sum_less(int left, int right, const T& upper) const {
        const int rank = static_cast<int>(
            std::lower_bound(coordinates_.begin(), coordinates_.end(), upper) -
            coordinates_.begin());
        return sum_rank_less(left, right, rank);
    }

    /// O(log sigma)。[left,right)にあるupper以下の要素の和を返す。
    Sum sum_leq(int left, int right, const T& upper) const {
        const int rank = static_cast<int>(
            std::upper_bound(coordinates_.begin(), coordinates_.end(), upper) -
            coordinates_.begin());
        return sum_rank_less(left, right, rank);
    }

    /// O(log sigma)。[left,right)にある[lower,upper)の要素の和を返す。
    Sum sum(int left, int right, const T& lower, const T& upper) const {
        return sum_less(left, right, upper) - sum_less(left, right, lower);
    }

    /// O(log sigma)。[left,right)でupper未満の最大値。なければnullopt。
    std::optional<T> prev(int left, int right, const T& upper) const {
        const int count = count_less(left, right, upper);
        if (count == 0) {
            return std::nullopt;
        }
        return kth(left, right, count - 1);
    }

    /// O(log sigma)。[left,right)でlower以上の最小値。なければnullopt。
    std::optional<T> next(int left, int right, const T& lower) const {
        const int count = count_less(left, right, lower);
        if (count == right - left) {
            return std::nullopt;
        }
        return kth(left, right, count);
    }

private:
    /// O(log sigma)。rankがrank_bound未満の要素数を返す。
    int count_rank_less(int left, int right, int rank_bound) const {
        assert_range(left, right);
        if (rank_bound <= 0) {
            return 0;
        }
        if (rank_bound >= static_cast<int>(coordinates_.size())) {
            return right - left;
        }
        int result = 0;
        for (int level = levels_ - 1; level >= 0; --level) {
            const int ones_left = one_prefix_[level][left];
            const int ones_right = one_prefix_[level][right];
            if ((rank_bound >> level) & 1) {
                result += (right - left) - (ones_right - ones_left);
                left = middle_[level] + ones_left;
                right = middle_[level] + ones_right;
            } else {
                left -= ones_left;
                right -= ones_right;
            }
        }
        return result;
    }

    /// O(log sigma)。rankがrank_bound未満の要素和を返す。
    Sum sum_rank_less(int left, int right, int rank_bound) const {
        assert_range(left, right);
        if (rank_bound <= 0) {
            return Sum{};
        }
        if (rank_bound >= static_cast<int>(coordinates_.size())) {
            return original_sum_[right] - original_sum_[left];
        }
        Sum result{};
        for (int level = levels_ - 1; level >= 0; --level) {
            const int ones_left = one_prefix_[level][left];
            const int ones_right = one_prefix_[level][right];
            if ((rank_bound >> level) & 1) {
                result += zero_sum_[level][right] - zero_sum_[level][left];
                left = middle_[level] + ones_left;
                right = middle_[level] + ones_right;
            } else {
                left -= ones_left;
                right -= ones_right;
            }
        }
        return result;
    }

    /// O(1)。半開区間を確認する。
    void assert_range(int left, int right) const {
        assert(0 <= left && left <= right && right <= size());
    }

    std::vector<T> values_;
    std::vector<T> coordinates_;
    int levels_ = 0;
    std::vector<int> middle_;
    std::vector<std::vector<int>> one_prefix_;
    std::vector<std::vector<Sum>> zero_sum_;
    std::vector<Sum> original_sum_;
};

}
