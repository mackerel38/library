#pragma once
#include <bits/stdc++.h>

namespace poe {

/// cartesiantree_resultはmin Cartesian Treeの親・左右の子・根を保持する。
struct cartesiantree_result {
    int root = -1;
    std::vector<int> parent;
    std::vector<int> left;
    std::vector<int> right;
};

/// O(n)。compare(child,parent)が偽となるCartesian Treeを返す。std::greater{}ならmax木。
/// compare上で同値なら左側の要素を祖先にする。
template <class T, class Compare = std::less<T>>
cartesiantree_result cartesian_tree(const std::vector<T>& values, Compare compare = {}) {
    const int n = static_cast<int>(values.size());
    cartesiantree_result result{
        .root = n == 0 ? -1 : 0,
        .parent = std::vector<int>(n, -1),
        .left = std::vector<int>(n, -1),
        .right = std::vector<int>(n, -1),
    };
    std::vector<int> stack;
    for (int index = 0; index < n; ++index) {
        int last = -1;
        while (!stack.empty() && compare(values[index], values[stack.back()])) {
            last = stack.back();
            stack.pop_back();
        }
        if (!stack.empty()) {
            result.parent[index] = stack.back();
            result.right[stack.back()] = index;
        }
        if (last != -1) {
            result.parent[last] = index;
            result.left[index] = last;
        }
        stack.push_back(index);
    }
    if (!stack.empty()) result.root = stack.front();
    return result;
}

}
