#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(h^3w^2+h^2w^3)時間・O(h^2w^2)領域。矩形の直接費用と直線分割から最小費用を返す。
template <class T, class Cost>
T minimum_rectangle_partition_cost(int height, int width, Cost cost) {
    assert(height >= 0 && width >= 0);
    if (height == 0 || width == 0) return T{};
    const int rows = height + 1;
    const int columns = width + 1;
    std::vector<T> dp(
        static_cast<std::size_t>(rows) * rows * columns * columns
    );
    const auto at = [&](int top, int bottom, int left, int right) -> T& {
        const std::size_t index =
            ((static_cast<std::size_t>(top) * rows + bottom) * columns + left) * columns + right;
        return dp[index];
    };

    for (int rectangle_height = 1; rectangle_height <= height; ++rectangle_height) {
        for (int rectangle_width = 1; rectangle_width <= width; ++rectangle_width) {
            for (int top = 0; top + rectangle_height <= height; ++top) {
                const int bottom = top + rectangle_height;
                for (int left = 0; left + rectangle_width <= width; ++left) {
                    const int right = left + rectangle_width;
                    T answer = cost(top, left, bottom, right);
                    for (int middle = top + 1; middle < bottom; ++middle) {
                        answer = std::min(answer, at(top, middle, left, right) +
                                                 at(middle, bottom, left, right));
                    }
                    for (int middle = left + 1; middle < right; ++middle) {
                        answer = std::min(answer, at(top, bottom, left, middle) +
                                                 at(top, bottom, middle, right));
                    }
                    at(top, bottom, left, right) = answer;
                }
            }
        }
    }
    return at(0, height, 0, width);
}

}
