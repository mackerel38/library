#pragma once
#include <bits/stdc++.h>

namespace poe {

/// thresholdsequence<long long> seq(up, down); seq.append(p); seq.apply(x)。追加ならしO(log n)、適用O(log^2 n)。
template<std::signed_integral T>
struct thresholdsequence {
    /// O(1)。各操作をx<=thresholdならx+up、そうでなければx-downとして空列を作る。
    thresholdsequence(T up, T down) : up_(up), down_(down) {
        assert(up > 0 && down > 0);
    }

    /// ならしO(log n)。末尾へ閾値thresholdの操作を追加する。
    void append(T threshold) {
        block current{{threshold}};
        while (!blocks_.empty() && blocks_.back().thresholds.size() == current.thresholds.size()) {
            current = merge(blocks_.back(), current);
            blocks_.pop_back();
        }
        blocks_.push_back(std::move(current));
        ++size_;
    }

    /// O(log^2 n)。valueへ現在の全操作を追加順に適用した値を返す。
    T apply(T value) const {
        for (const auto& current : blocks_) value = apply(current, value);
        return value;
    }

    /// O(1)。追加済み操作数を返す。
    std::size_t size() const noexcept { return size_; }

    /// O(1)。操作が空ならtrueを返す。
    bool empty() const noexcept { return size_ == 0; }

private:
    struct block {
        std::vector<T> thresholds;
    };

    T up_;
    T down_;
    std::size_t size_ = 0;
    std::vector<block> blocks_;

    T apply(const block& current, T value) const {
        std::size_t left = 0;
        std::size_t right = current.thresholds.size();
        while (left < right) {
            const std::size_t middle = std::midpoint(left, right);
            if (value > current.thresholds[middle] + static_cast<T>(middle) * down_) {
                left = middle + 1;
            } else {
                right = middle;
            }
        }
        const T decrease_count = static_cast<T>(left);
        const T length = static_cast<T>(current.thresholds.size());
        return value + up_ * length - (up_ + down_) * decrease_count;
    }

    block merge(const block& first, const block& second) const {
        const T first_size = static_cast<T>(first.thresholds.size());
        const T second_size = static_cast<T>(second.thresholds.size());
        block result;
        result.thresholds.reserve(first.thresholds.size() + second.thresholds.size());
        T left = 0;
        T right = 0;
        while (left < first_size || right < second_size) {
            const bool take_second = left == first_size
                || (right < second_size
                    && first.thresholds[left] - right * down_
                        > second.thresholds[right] - (first_size - left) * up_);
            if (take_second) {
                result.thresholds.push_back(second.thresholds[right++] - (first_size - left) * up_);
            } else {
                T threshold = first.thresholds[left++] - right * down_;
                if (!result.thresholds.empty()) threshold = std::max(threshold, result.thresholds.back() + up_);
                result.thresholds.push_back(threshold);
            }
        }
        return result;
    }
};

}
