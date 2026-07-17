#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 端点重複なしのlaminar閉区間族: laminarintervalset data(coordinate_count)。
struct laminarintervalset {
    /// O(n)。座標[0,coordinate_count)上の空の区間族を作る。
    explicit laminarintervalset(int coordinate_count)
        : coordinate_count_(coordinate_count), endpoint_used_(coordinate_count) {
        assert(coordinate_count >= 0);
        while (size_ < coordinate_count_) size_ <<= 1;
        lazy_.assign(2 * size_, none());
    }

    /// O(log n)。閉区間[left,right]が既存区間と交差しなければ追加してtrue、交差すればfalse。
    bool add(int left, int right) {
        assert(0 <= left && left < right && right < coordinate_count_);
        assert(!endpoint_used_[left] && !endpoint_used_[right]);
        if (!compatible(left, right) || !compatible(right, left)) return false;
        const interval value{right - left, left, right};
        apply(left, right + 1, value);
        endpoint_used_[left] = endpoint_used_[right] = true;
        intervals_.emplace_back(left, right);
        return true;
    }

    /// O(1)。受理済み区間数を返す。
    int size() const noexcept {
        return static_cast<int>(intervals_.size());
    }

    /// O(1)。追加順の受理済み閉区間列を返す。
    const std::vector<std::pair<int, int>>& intervals() const noexcept {
        return intervals_;
    }

private:
    struct interval {
        int length;
        int left;
        int right;
        auto operator<=>(const interval&) const = default;
    };

    static interval none() {
        return {std::numeric_limits<int>::max(), -1, -1};
    }

    bool compatible(int endpoint, int other) const {
        const interval containing = get(endpoint);
        return containing.left == -1
            || (containing.left < other && other < containing.right);
    }

    interval get(int index) const {
        interval result = none();
        for (int node = index + size_; node > 0; node >>= 1) {
            result = std::min(result, lazy_[node]);
        }
        return result;
    }

    void apply(int left, int right, const interval& value) {
        for (left += size_, right += size_; left < right; left >>= 1, right >>= 1) {
            if (left & 1) lazy_[left] = std::min(lazy_[left], value), ++left;
            if (right & 1) --right, lazy_[right] = std::min(lazy_[right], value);
        }
    }

    int coordinate_count_;
    int size_ = 1;
    std::vector<interval> lazy_;
    std::vector<bool> endpoint_used_;
    std::vector<std::pair<int, int>> intervals_;
};

}
