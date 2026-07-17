#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 多次元累積変換の向き。prefixは各座標以下、suffixは各座標以上を集約する。
enum class cumulativedirection { prefix, suffix };

/// O(dN)。row-major多次元配列を各座標以下または以上のoperation累積へ変換する。
template<class T, class Operation>
std::vector<T> multidimensional_cumulative(
    const std::vector<int>& shape,
    std::vector<T> values,
    Operation operation,
    cumulativedirection direction = cumulativedirection::prefix
) {
    assert(!shape.empty());
    long long total = 1;
    for (int extent : shape) {
        assert(extent >= 0);
        total *= extent;
        assert(total <= std::numeric_limits<int>::max());
    }
    assert(static_cast<int>(values.size()) == total);

    std::vector<int> strides(shape.size());
    int stride = 1;
    for (int dimension = static_cast<int>(shape.size()) - 1; dimension >= 0; --dimension) {
        strides[dimension] = stride;
        stride *= shape[dimension];
    }
    for (int dimension = 0; dimension < static_cast<int>(shape.size()); ++dimension) {
        const int current_stride = strides[dimension];
        if (direction == cumulativedirection::prefix) {
            for (int index = 0; index < static_cast<int>(values.size()); ++index) {
                const int coordinate = index / current_stride % shape[dimension];
                if (coordinate != 0) {
                    values[index] = operation(values[index], values[index - current_stride]);
                }
            }
        } else {
            for (int index = static_cast<int>(values.size()) - 1; index >= 0; --index) {
                const int coordinate = index / current_stride % shape[dimension];
                if (coordinate + 1 < shape[dimension]) {
                    values[index] = operation(values[index], values[index + current_stride]);
                }
            }
        }
    }
    return values;
}

}
