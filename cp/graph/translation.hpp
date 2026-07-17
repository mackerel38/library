#pragma once
#include <bits/stdc++.h>
#include "graph/grid.hpp"

namespace poe {

namespace detail {

struct translationstate {
    int top;
    int bottom;
    int left;
    int right;
    int row_shift;
    int column_shift;
    auto operator<=>(const translationstate&) const = default;
};

struct translationstatehash {
    std::size_t operator()(const translationstate& state) const noexcept {
        std::size_t result = 0;
        for (const int value : std::array{
                 state.top, state.bottom, state.left, state.right,
                 state.row_shift, state.column_shift
             }) {
            result ^= std::hash<int>{}(value) + 0x9e3779b9U + (result << 6) + (result >> 2);
        }
        return result;
    }
};

}

/// O(h^3 w^3 K(F+1))。点集合を同時移動し、禁止点を踏まず盤外へ全消去する最小回数を返す。
template <std::size_t K>
int minimum_safe_translation_clear_steps(
    int height,
    int width,
    const std::vector<gridpoint>& occupied,
    const std::vector<gridpoint>& forbidden,
    const std::array<gridpoint, K>& directions
) {
    assert(height >= 0 && width >= 0);
    std::vector initial(height, std::vector<int>(width));
    for (const auto point : occupied) {
        assert(inside_grid(point.row, point.column, height, width));
        initial[point.row][point.column] = 1;
    }
    for (const auto point : forbidden) {
        assert(inside_grid(point.row, point.column, height, width));
    }
    std::vector prefix(height + 1, std::vector<int>(width + 1));
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            prefix[row + 1][column + 1] =
                prefix[row][column + 1] + prefix[row + 1][column]
                - prefix[row][column] + initial[row][column];
        }
    }
    auto count = [&](const detail::translationstate& state) {
        return prefix[state.bottom][state.right] - prefix[state.top][state.right]
             - prefix[state.bottom][state.left] + prefix[state.top][state.left];
    };
    auto safe = [&](const detail::translationstate& state) {
        for (const auto point : forbidden) {
            const int original_row = point.row - state.row_shift;
            const int original_column = point.column - state.column_shift;
            if (state.top <= original_row && original_row < state.bottom &&
                state.left <= original_column && original_column < state.right &&
                initial[original_row][original_column]) {
                return false;
            }
        }
        return true;
    };

    const detail::translationstate start{0, height, 0, width, 0, 0};
    if (!safe(start)) return -1;
    if (count(start) == 0) return 0;
    std::unordered_map<
        detail::translationstate,
        int,
        detail::translationstatehash
    > distance;
    distance.emplace(start, 0);
    std::queue<detail::translationstate> queue;
    queue.push(start);
    while (!queue.empty()) {
        const auto state = queue.front();
        queue.pop();
        const int current_distance = distance.find(state)->second;
        for (const auto direction : directions) {
            auto next = state;
            next.row_shift += direction.row;
            next.column_shift += direction.column;
            next.top = std::max(next.top, -next.row_shift);
            next.bottom = std::min(next.bottom, height - next.row_shift);
            next.left = std::max(next.left, -next.column_shift);
            next.right = std::min(next.right, width - next.column_shift);
            if (next.top >= next.bottom || next.left >= next.right) {
                return current_distance + 1;
            }
            if (!safe(next) || distance.contains(next)) continue;
            if (count(next) == 0) return current_distance + 1;
            distance.emplace(next, current_distance + 1);
            queue.push(next);
        }
    }
    return -1;
}

/// O(h^3 w^3 F)。上下左右への一斉移動で全点を安全に消す最小回数を返す。
inline int minimum_safe_translation_clear_steps(
    int height,
    int width,
    const std::vector<gridpoint>& occupied,
    const std::vector<gridpoint>& forbidden
) {
    return minimum_safe_translation_clear_steps(
        height, width, occupied, forbidden, grid4
    );
}

}
