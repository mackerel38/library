#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(p(n)・n)。S_nの置換のcycle個数列から固定点数を返すfixed_countを用いて軌道数を返す。
template<class mint, class FixedCount>
mint symmetric_group_orbit_count(int size, FixedCount fixed_count) {
    assert(size >= 0);
    std::vector<int> cycle_counts(size + 1);
    std::vector<mint> inverse(size + 1, mint{1});
    for (int value = 1; value <= size; ++value) inverse[value] = mint{1} / mint{value};

    auto enumerate = [&](auto&& self, int length, int remaining, mint weight) -> mint {
        if (remaining == 0) return weight * fixed_count(cycle_counts);
        if (length > remaining) return mint{};
        mint answer{};
        mint current_weight = weight;
        for (int count = 0; count * length <= remaining; ++count) {
            cycle_counts[length] = count;
            answer += self(self, length + 1, remaining - count * length, current_weight);
            if ((count + 1) * length <= remaining) {
                current_weight *= inverse[length] * inverse[count + 1];
            }
        }
        cycle_counts[length] = 0;
        return answer;
    };
    return enumerate(enumerate, 1, size, mint{1});
}

}
