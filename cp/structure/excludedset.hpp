#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 固定された除外整数集合: excludedset<long long> data(values)。
template <std::integral T>
struct excludedset {
    /// O(n log n)。valuesを除外集合として構築し、重複を取り除く。
    explicit excludedset(std::vector<T> values) : values_(std::move(values)) {
        std::ranges::sort(values_);
        values_.erase(std::unique(values_.begin(), values_.end()), values_.end());
    }

    /// O(log n)。valueが除外されているならtrueを返す。
    bool contains(T value) const {
        return std::ranges::binary_search(values_, value);
    }

    /// O(log n)。lower以上かつ除外されていない整数のうちrank番目を返す。rankは1以上。
    T kth_allowed(T lower, T rank) const {
        assert(rank >= 1);
        const std::size_t first = std::lower_bound(values_.begin(), values_.end(), lower)
            - values_.begin();
        std::size_t ng = first;
        std::size_t ok = values_.size();
        while (ng < ok) {
            const std::size_t middle = ng + (ok - ng) / 2;
            const T excluded_count = static_cast<T>(middle - first + 1);
            const T allowed_count = values_[middle] - lower + T{1} - excluded_count;
            if (allowed_count >= rank) ok = middle;
            else ng = middle + 1;
        }
        return lower + (rank - T{1}) + static_cast<T>(ok - first);
    }

    /// O(1)。除外整数の個数を返す。
    int size() const noexcept {
        return static_cast<int>(values_.size());
    }

    /// O(1)。昇順の除外整数列を返す。
    const std::vector<T>& values() const noexcept {
        return values_;
    }

private:
    std::vector<T> values_;
};

}
