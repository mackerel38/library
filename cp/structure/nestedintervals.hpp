#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 長さ昇順の入れ子区間列をprefix左端・右端整列で管理する: nestedintervals<long long> a(lengths)。
template <class T>
struct nestedintervals {
    /// O(n)。厳密増加するlengthsの区間[0,lengths[i]]から構築する。
    explicit nestedintervals(std::vector<T> lengths) : lengths_(std::move(lengths)) {
        for (int i = 0; i < size(); ++i) {
            assert(lengths_[i] > T{});
            if (i) assert(lengths_[i - 1] < lengths_[i]);
        }
        if (!lengths_.empty()) blocks_.push_back({0, size() - 1, false, T{}});
    }

    /// O(1)。区間数を返す。
    int size() const noexcept { return static_cast<int>(lengths_.size()); }

    /// O(log n)。index番目の[left,right]を返す。
    std::pair<T, T> interval(int index) const {
        assert_index(index);
        const auto& block = blocks_[block_index(index)];
        if (block.right_aligned) return {block.anchor - lengths_[index], block.anchor};
        return {block.anchor, block.anchor + lengths_[index]};
    }

    /// O(log n)。a[index]はindex番目の[left,right]を返す。
    std::pair<T, T> operator[](int index) const { return interval(index); }

    /// 償却O(log n)。[0,last]の左端を現在の区間lastの左端へ揃える。
    void align_left(int last) { set_prefix_left(last, interval(last).first); }

    /// 償却O(log n)。[0,last]の右端を現在の区間lastの右端へ揃える。
    void align_right(int last) { set_prefix_right(last, interval(last).second); }

    /// 償却O(1)、最悪O(n)。[0,last]の左端をleftへ揃える。
    void set_prefix_left(int last, const T& left) { replace_prefix(last, false, left); }

    /// 償却O(1)、最悪O(n)。[0,last]の右端をrightへ揃える。
    void set_prefix_right(int last, const T& right) { replace_prefix(last, true, right); }

    /// O(log n)。半開区間[left,right)がcoordinateを含む区間数を返す。
    int count_covering(const T& coordinate) const {
        if (blocks_.empty()) return 0;
        int low = -1;
        int high = static_cast<int>(blocks_.size());
        while (high - low > 1) {
            const int middle = std::midpoint(low, high);
            if (contains(blocks_[middle], blocks_[middle].right, coordinate)) low = middle;
            else high = middle;
        }
        if (low == -1) return 0;
        const auto& block = blocks_[low];
        int ng = block.left - 1;
        int ok = block.right;
        while (ok - ng > 1) {
            const int middle = std::midpoint(ng, ok);
            if (contains(block, middle, coordinate)) ok = middle;
            else ng = middle;
        }
        return size() - ok;
    }

private:
    struct block {
        int left;
        int right;
        bool right_aligned;
        T anchor;
    };

    bool contains(const block& current, int index, const T& coordinate) const {
        const auto [left, right] = current.right_aligned
            ? std::pair{current.anchor - lengths_[index], current.anchor}
            : std::pair{current.anchor, current.anchor + lengths_[index]};
        return left <= coordinate && coordinate < right;
    }

    int block_index(int index) const {
        int low = 0;
        int high = static_cast<int>(blocks_.size());
        while (low < high) {
            const int middle = std::midpoint(low, high);
            if (blocks_[middle].left <= index) high = middle;
            else low = middle + 1;
        }
        assert(low < static_cast<int>(blocks_.size()) && index <= blocks_[low].right);
        return low;
    }

    void replace_prefix(int last, bool right_aligned, const T& anchor) {
        assert_index(last);
        while (blocks_.back().right < last) blocks_.pop_back();
        auto current = blocks_.back();
        blocks_.pop_back();
        if (last < current.right) {
            current.left = last + 1;
            blocks_.push_back(current);
        }
        blocks_.push_back({0, last, right_aligned, anchor});
    }

    void assert_index(int index) const { assert(0 <= index && index < size()); }

    std::vector<T> lengths_;
    std::vector<block> blocks_;
};

}
