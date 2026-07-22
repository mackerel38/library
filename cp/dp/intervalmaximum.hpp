#pragma once
#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "utility/presetsegtree.hpp"

namespace poe {

/// count_interval_maximum_sequencesへ渡す半開区間[left,right)の最大値制約。
struct intervalmaximumconstraint {
    int left;
    int right;
    int maximum;
};

/// O((n+q) log(n+q))時間・O(n+q)領域。0..maximum_valueの列で全区間最大値制約を満たす個数。
template <int mod>
staticmodint<mod> count_interval_maximum_sequences(
    int size,
    int maximum_value,
    const std::vector<intervalmaximumconstraint>& constraints
) {
    using mint = staticmodint<mod>;
    assert(size >= 0 && 0 <= maximum_value && maximum_value < mod);
    if (size == 0) return constraints.empty() ? mint{1} : mint{};

    std::vector<std::vector<int>> begin(size + 1), end(size + 1);
    for (int index = 0; index < static_cast<int>(constraints.size()); ++index) {
        const auto [left, right, maximum] = constraints[index];
        assert(0 <= left && left < right && right <= size);
        assert(0 <= maximum && maximum <= maximum_value);
        begin[left].push_back(index);
        end[right].push_back(index);
    }

    std::multiset<int> active;
    std::map<int, std::vector<int>> positions;
    for (int position = 0; position < size; ++position) {
        for (const int index : end[position]) {
            const auto iterator = active.find(constraints[index].maximum);
            assert(iterator != active.end());
            active.erase(iterator);
        }
        for (const int index : begin[position]) active.insert(constraints[index].maximum);
        positions[active.empty() ? maximum_value : *active.begin()].push_back(position);
    }

    std::map<int, std::vector<intervalmaximumconstraint>> grouped_constraints;
    for (const auto& constraint : constraints) {
        grouped_constraints[constraint.maximum].push_back(constraint);
    }

    mint answer = 1;
    for (const auto& [upper, candidate_positions] : positions) {
        const int count = static_cast<int>(candidate_positions.size());
        std::vector<int> required_left(count, -1);
        if (const auto group = grouped_constraints.find(upper);
            group != grouped_constraints.end()) {
            for (const auto& constraint : group->second) {
                const int left = static_cast<int>(std::lower_bound(
                    candidate_positions.begin(), candidate_positions.end(), constraint.left
                ) - candidate_positions.begin());
                const int right = static_cast<int>(std::lower_bound(
                    candidate_positions.begin(), candidate_positions.end(), constraint.right
                ) - candidate_positions.begin());
                if (left == right) return 0;
                required_left[right - 1] = std::max(required_left[right - 1], left);
            }
        }

        std::vector<mint> initial(static_cast<std::size_t>(count) + 1);
        initial[0] = 1;
        range_affine_range_sum<mint> dp(initial);
        for (int index = 0; index < count; ++index) {
            const mint total = dp.sum();
            dp.affine(0, index + 1, mint{upper}, mint{});
            dp.set(index + 1, total);
            if (required_left[index] != -1) {
                dp.affine(0, required_left[index] + 1, mint{}, mint{});
            }
        }
        answer *= dp.sum();
    }

    for (const auto& [maximum, group] : grouped_constraints) {
        if (!positions.contains(maximum)) return 0;
    }
    return answer;
}

}
