#pragma once
#include <bits/stdc++.h>
#include "structure/linecontainer.hpp"

namespace poe {

/// Li Chao Treeが最小値・最大値のどちらを返すかを表す。
enum class lichaomode { minimum, maximum };

/// 永続動的Li Chao Tree: versionはint handle。直線追加・取得O(depth)、copy O(1)。
template <class T, lichaomode Mode = lichaomode::minimum>
struct persistentlichaotree {
    using version = int;

    /// O(1)。query範囲[left,right]と最大深さを指定して空treeを作る。
    persistentlichaotree(T left, T right, int depth = 60)
        : left_(left), right_(right), depth_(depth) {
        assert(left < right && depth >= 1);
    }

    /// O(1)。直線を持たない初期versionを返す。
    version initial() const noexcept { return -1; }

    /// O(depth)。baseへvalueを追加した新versionを返し、baseは変更しない。
    version add_line(version base, line<T> value) {
        assert(base == -1 || (0 <= base && base < static_cast<int>(nodes_.size())));
        return add_node(base, value, left_, right_, 0);
    }

    /// O(depth)。指定versionに含まれる直線のxでの最小値または最大値を返す。
    T get(version root, T x) const {
        assert(0 <= root && root < static_cast<int>(nodes_.size()));
        assert(left_ <= x && x <= right_);
        std::optional<T> answer;
        T left = left_, right = right_;
        for (int level = 0; root != -1 && level <= depth_; ++level) {
            const auto& current = nodes_[root];
            if (current.value) {
                const T candidate = current.value->get(x);
                if (!answer || better(candidate, *answer)) answer = candidate;
            }
            const T middle = std::midpoint(left, right);
            if (x <= middle) {
                root = current.left;
                right = middle;
            } else {
                root = current.right;
                left = middle;
            }
        }
        assert(answer.has_value());
        return *answer;
    }

    /// O(1)。versionが空ならtrueを返す。
    bool empty(version root) const noexcept { return root == -1; }

    /// O(1)。確保済みnode数を返す。
    int node_count() const noexcept { return static_cast<int>(nodes_.size()); }

    /// O(count)。node再確保を減らすため領域を予約する。
    void reserve_nodes(std::size_t count) { nodes_.reserve(count); }

private:
    struct node {
        std::optional<line<T>> value;
        int left = -1;
        int right = -1;
    };

    static bool better(const T& first, const T& second) {
        if constexpr (Mode == lichaomode::minimum) return first < second;
        else return first > second;
    }

    version clone(version root) {
        nodes_.push_back(root == -1 ? node{} : nodes_[root]);
        return static_cast<int>(nodes_.size()) - 1;
    }

    version add_node(version root, line<T> value, T left, T right, int level) {
        const version result = clone(root);
        auto& current = nodes_[result];
        if (!current.value) {
            current.value = value;
            return result;
        }
        const T middle = std::midpoint(left, right);
        const bool better_left = better(value.get(left), current.value->get(left));
        const bool better_middle = better(value.get(middle), current.value->get(middle));
        if (better_middle) std::swap(value, *current.value);
        if (level == depth_) return result;
        if (better_left != better_middle) {
            const int child = add_node(current.left, value, left, middle, level + 1);
            nodes_[result].left = child;
        } else {
            const int child = add_node(current.right, value, middle, right, level + 1);
            nodes_[result].right = child;
        }
        return result;
    }

    T left_;
    T right_;
    int depth_;
    std::vector<node> nodes_;
};

}
