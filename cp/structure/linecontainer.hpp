#pragma once
#include <bits/stdc++.h>

namespace poe {

/// y=ax+bを表す直線。
template <class T>
struct line {
    T slope{};
    T intercept{};

    /// O(1)。xでの値を返す。
    T get(const T& x) const { return slope * x + intercept; }
};

namespace detail {

template <class T, class Compare>
struct discrete_linecontainer {
    explicit discrete_linecontainer(std::vector<T> coordinates, Compare compare = {})
        : x_(std::move(coordinates)), compare_(compare), tree_(x_.size() * 4) {
        std::ranges::sort(x_);
        x_.erase(std::unique(x_.begin(), x_.end()), x_.end());
        tree_.assign(x_.size() * 4, std::nullopt);
    }

    int size() const noexcept { return static_cast<int>(x_.size()); }

    void add_line(line<T> value) {
        assert(!x_.empty());
        add_line_node(value, 1, 0, size());
    }

    void add_segment(int left, int right, line<T> value) {
        assert(!x_.empty());
        assert(0 <= left && left <= right && right <= size());
        add_segment_node(left, right, value, 1, 0, size());
    }

    T get(const T& x) const {
        const int index = static_cast<int>(std::lower_bound(x_.begin(), x_.end(), x) - x_.begin());
        assert(index < size() && x_[index] == x);
        std::optional<T> answer;
        int node = 1, left = 0, right = size();
        while (true) {
            if (tree_[node]) {
                const T value = tree_[node]->get(x);
                if (!answer || compare_(value, *answer)) answer = value;
            }
            if (right - left == 1) break;
            const int middle = (left + right) / 2;
            if (index < middle) { node *= 2; right = middle; }
            else { node = node * 2 + 1; left = middle; }
        }
        assert(answer.has_value());
        return *answer;
    }

private:
    void add_line_node(line<T> value, int node, int left, int right) {
        if (!tree_[node]) { tree_[node] = value; return; }
        const int middle = (left + right) / 2;
        const bool better_left = compare_(value.get(x_[left]), tree_[node]->get(x_[left]));
        const bool better_middle = compare_(value.get(x_[middle]), tree_[node]->get(x_[middle]));
        if (better_middle) std::swap(value, *tree_[node]);
        if (right - left == 1) return;
        if (better_left != better_middle) add_line_node(value, node * 2, left, middle);
        else add_line_node(value, node * 2 + 1, middle, right);
    }

    void add_segment_node(int query_left, int query_right, line<T> value, int node, int left, int right) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            add_line_node(value, node, left, right);
            return;
        }
        const int middle = (left + right) / 2;
        add_segment_node(query_left, query_right, value, node * 2, left, middle);
        add_segment_node(query_left, query_right, value, node * 2 + 1, middle, right);
    }

    std::vector<T> x_;
    Compare compare_;
    std::vector<std::optional<line<T>>> tree_;
};

}

/// 最小値Li Chao Tree: minlinecontainer<long long> lines(xs); 追加O(log n)、取得O(log n)。
template <class T>
using minlinecontainer = detail::discrete_linecontainer<T, std::less<T>>;

/// 最大値Li Chao Tree: maxlinecontainer<long long> lines(xs); 追加O(log n)、取得O(log n)。
template <class T>
using maxlinecontainer = detail::discrete_linecontainer<T, std::greater<T>>;

}
