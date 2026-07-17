#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(columns log rows + rows)。monotone_minima<T>(rows, columns, value): 各行の最小値と列を返す。
/// 最小列が行に対して広義単調増加であること。同値なら最小の列を選ぶ。
template<class T, class Value>
std::vector<std::pair<T, int>> monotone_minima(int rows, int columns, Value value) {
    assert(rows >= 0 && columns > 0);
    std::vector<std::pair<T, int>> answer(rows);
    auto solve = [&](auto&& self, int top, int bottom, int left, int right) -> void {
        if (top >= bottom) return;
        const int middle = (top + bottom) / 2;
        std::optional<std::pair<T, int>> best;
        for (int column = left; column < right; ++column) {
            const std::pair<T, int> candidate{value(middle, column), column};
            if (!best || candidate < *best) best = candidate;
        }
        assert(best.has_value());
        answer[middle] = *best;
        self(self, top, middle, left, best->second + 1);
        self(self, middle + 1, bottom, best->second, right);
    };
    solve(solve, 0, rows, 0, columns);
    return answer;
}

}
