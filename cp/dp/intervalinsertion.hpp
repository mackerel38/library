#pragma once
#include <bits/stdc++.h>
#include "math/combination.hpp"

namespace poe {

/// 一回の挿入で区別される選択肢数countと、その即時費用の総和cost。
template<class T>
struct intervalinsertionchoice {
    T count;
    T cost;
};

/// 区間内の全挿入順と選択肢について、場合の数と費用総和を保持する。
template<class T>
struct intervalinsertionaggregate {
    int size;
    std::vector<std::vector<T>> count;
    std::vector<std::vector<T>> cost;

    /// O(1)。端点left,rightの間を埋める場合の数を返す。
    const T& count_at(int left, int right) const {
        assert(0 <= left && left < right && right < size);
        return count[left][right];
    }

    /// O(1)。端点left,rightの間を埋める全場合の費用総和を返す。
    const T& cost_at(int left, int right) const {
        assert(0 <= left && left < right && right < size);
        return cost[left][right];
    }
};

/// O(size*maximum_span^2)。両端から始めて区間内部を挿入する全順序の重みと費用を集約する。
template<class T, class Choice>
intervalinsertionaggregate<T> interval_insertion_aggregate(
    int size,
    int maximum_span,
    Choice choice
) {
    assert(size >= 1);
    assert(0 <= maximum_span && maximum_span < size);
    combination<T> comb(maximum_span);
    intervalinsertionaggregate<T> result{
        size,
        std::vector(size, std::vector<T>(size)),
        std::vector(size, std::vector<T>(size))
    };
    for (int left = 0; left + 1 < size; ++left) result.count[left][left + 1] = T{1};
    for (int span = 2; span <= maximum_span; ++span) {
        for (int left = 0, right = span; right < size; ++left, ++right) {
            for (int middle = left + 1; middle < right; ++middle) {
                const auto current = choice(left, middle, right);
                const T interleavings = comb.choose(span - 2, middle - left - 1);
                const T pair_count = result.count[left][middle] * result.count[middle][right];
                result.count[left][right] += interleavings * current.count * pair_count;
                result.cost[left][right] += interleavings * (
                    current.cost * pair_count
                    + current.count * (
                        result.cost[left][middle] * result.count[middle][right]
                        + result.count[left][middle] * result.cost[middle][right]
                    )
                );
            }
        }
    }
    return result;
}

/// O(n^3)時間・O(n^2)領域。円環で向く側の最近傍を一様な生存者が削除する総距離の期待値。
template<class T>
T circular_directional_removal_expectation(std::string_view directions) {
    const int n = static_cast<int>(directions.size());
    assert(n >= 2);
    for (const char direction : directions) assert(direction == 'L' || direction == 'R');
    const auto aggregate = interval_insertion_aggregate<T>(
        2 * n, n,
        [&](int left, int middle, int right) {
            intervalinsertionchoice<T> result{};
            if (directions[left % n] == 'R') {
                result.count += 1;
                result.cost += middle - left;
            }
            if (directions[right % n] == 'L') {
                result.count += 1;
                result.cost += right - middle;
            }
            return result;
        }
    );
    T total{};
    for (int root = 0; root < n; ++root) total += aggregate.cost_at(root, root + n);
    return total / combination<T>(n).factorial(n);
}

}
