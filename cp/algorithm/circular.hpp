#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n log n)。各値にmodulusの倍数を足し引きしてmax-minを最小化した値を返す。
template <std::integral T>
T minimum_modular_range(std::vector<T> values, T modulus) {
    assert(modulus > 0);
    if (values.size() <= 1) return 0;
    for (T& value : values) {
        value %= modulus;
        if (value < 0) value += modulus;
    }
    std::ranges::sort(values);
    T maximum_gap = values.front() + modulus - values.back();
    for (int i = 1; i < static_cast<int>(values.size()); ++i)
        maximum_gap = std::max(maximum_gap, values[i] - values[i - 1]);
    return modulus - maximum_gap;
}

}
