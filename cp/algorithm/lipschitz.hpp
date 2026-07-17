#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n)。upper以下で隣接差の絶対値がslope以下となる最大の列を返す。
template <class T>
std::vector<T> lipschitz_minorant(std::vector<T> upper, T slope) {
    assert(slope >= T{});
    for (int index = 1; index < static_cast<int>(upper.size()); ++index) {
        upper[index] = std::min(upper[index], upper[index - 1] + slope);
    }
    for (int index = static_cast<int>(upper.size()) - 2; index >= 0; --index) {
        upper[index] = std::min(upper[index], upper[index + 1] + slope);
    }
    return upper;
}

}
