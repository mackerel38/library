#pragma once
#include <bits/stdc++.h>
#include "math/combination.hpp"

namespace poe {

/// O(boxes*types)時間・O(boxes+types)領域。各箱へ非空な種類集合を置き、種類別使用箱数が上限以下の方法数。
template<class T>
T count_nonempty_subset_occupancies(
    int boxes,
    const std::vector<int>& capacities
) {
    assert(boxes >= 0);
    for (const int capacity : capacities) assert(0 <= capacity && capacity <= boxes);
    if (boxes == 0) return T{1};
    if (capacities.empty()) return T{};
    combination<T> comb(boxes);
    std::vector<T> prefix(capacities.size(), T{1});
    T answer{};
    for (int available = 0; available <= boxes; ++available) {
        T ways = comb.choose(boxes, available);
        for (const T& count : prefix) ways *= count;
        if ((boxes - available) % 2 == 0) answer += ways;
        else answer -= ways;
        if (available == boxes) break;
        for (int type = 0; type < static_cast<int>(capacities.size()); ++type) {
            prefix[type] = prefix[type] * 2 - comb.choose(available, capacities[type]);
        }
    }
    return answer;
}

}
