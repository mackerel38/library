#pragma once
#include <bits/stdc++.h>

namespace poe {
namespace detail {

struct rangesmallesttree {
    struct node {
        std::vector<std::pair<long long, int>> smallest;
        long long lazy = 0;
    };

    int size;
    int keep;
    std::vector<node> tree;
    static constexpr long long infinity = (1LL << 60);

    rangesmallesttree(int size_, int keep_) : size(size_), keep(keep_), tree(4 * size_) {
        build(1, 0, size);
    }

    void add(int left, int right, long long value) { add(1, 0, size, left, right, value); }
    void set(int position, long long value) { set(1, 0, size, position, value); }

    long long count_at_most(long long limit) const {
        long long result = 0;
        for (const auto& [value, count] : tree[1].smallest) {
            if (value <= limit) result += count;
        }
        return result;
    }

private:
    void build(int index, int left, int right) {
        if (right - left == 1) {
            tree[index].smallest = {{infinity, 1}};
            return;
        }
        const int middle = (left + right) / 2;
        build(2 * index, left, middle);
        build(2 * index + 1, middle, right);
        pull(index);
    }

    void apply(int index, long long value) {
        tree[index].lazy += value;
        for (auto& [current, count] : tree[index].smallest) {
            static_cast<void>(count);
            current += value;
        }
    }

    void push(int index) {
        if (tree[index].lazy == 0) return;
        apply(2 * index, tree[index].lazy);
        apply(2 * index + 1, tree[index].lazy);
        tree[index].lazy = 0;
    }

    void pull(int index) {
        tree[index].smallest.clear();
        const auto& left = tree[2 * index].smallest;
        const auto& right = tree[2 * index + 1].smallest;
        std::size_t i = 0, j = 0;
        while (tree[index].smallest.size() < static_cast<std::size_t>(keep)
               && (i < left.size() || j < right.size())) {
            const long long value = j == right.size() || (i < left.size() && left[i].first < right[j].first)
                ? left[i].first : right[j].first;
            int count = 0;
            if (i < left.size() && left[i].first == value) count += left[i++].second;
            if (j < right.size() && right[j].first == value) count += right[j++].second;
            tree[index].smallest.emplace_back(value, count);
        }
    }

    void add(int index, int left, int right, int query_left, int query_right, long long value) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            apply(index, value);
            return;
        }
        push(index);
        const int middle = (left + right) / 2;
        add(2 * index, left, middle, query_left, query_right, value);
        add(2 * index + 1, middle, right, query_left, query_right, value);
        pull(index);
    }

    void set(int index, int left, int right, int position, long long value) {
        if (right - left == 1) {
            tree[index].smallest = {{value, 1}};
            tree[index].lazy = 0;
            return;
        }
        push(index);
        const int middle = (left + right) / 2;
        if (position < middle) set(2 * index, left, middle, position, value);
        else set(2 * index + 1, middle, right, position, value);
        pull(index);
    }
};

}

/// O(n(k+1)log n)時間・O(n(k+1))領域。相異なる整数列でmax-min-(長さ-1)<=kとなる部分配列数を返す。
template<std::integral Integer>
long long count_subarrays_range_excess_at_most(
    const std::vector<Integer>& values,
    int k
) {
    assert(k >= 0);
    if (values.empty()) return 0;
    std::vector<Integer> sorted = values;
    std::ranges::sort(sorted);
    assert(std::ranges::adjacent_find(sorted) == sorted.end());

    const int size = static_cast<int>(values.size());
    detail::rangesmallesttree data(size, k + 1);
    std::vector<int> maximum_stack, minimum_stack;
    long long answer = 0;
    for (int right = 0; right < size; ++right) {
        data.add(0, right, -1);
        while (!maximum_stack.empty() && values[maximum_stack.back()] < values[right]) {
            const int index = maximum_stack.back();
            maximum_stack.pop_back();
            const int left = maximum_stack.empty() ? 0 : maximum_stack.back() + 1;
            data.add(left, index + 1,
                     static_cast<long long>(values[right]) - values[index]);
        }
        maximum_stack.push_back(right);
        while (!minimum_stack.empty() && values[minimum_stack.back()] > values[right]) {
            const int index = minimum_stack.back();
            minimum_stack.pop_back();
            const int left = minimum_stack.empty() ? 0 : minimum_stack.back() + 1;
            data.add(left, index + 1,
                     static_cast<long long>(values[index]) - values[right]);
        }
        minimum_stack.push_back(right);
        data.set(right, 0);
        answer += data.count_at_most(k);
    }
    return answer;
}

}
