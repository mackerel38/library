#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O((n+k) prod(target[c]+1))。順番固定のk種類の要素を、種類別人数を指定して二群へ最小費用で分ける。
/// cost(index, group, first_count, second_count)はindexを置く直前の人数から追加費用を返す。
template <class T, std::size_t Categories, class Cost>
T minimum_two_group_assignment(
    const std::vector<int>& category,
    const std::array<int, Categories>& first_target,
    Cost cost,
    T infinity
) {
    static_assert(Categories > 0);
    std::array<int, Categories> total{};
    for (const int value : category) {
        assert(0 <= value && value < static_cast<int>(Categories));
        ++total[value];
    }

    std::array<std::size_t, Categories> stride{};
    std::size_t state_count = 1;
    for (std::size_t kind = 0; kind < Categories; ++kind) {
        assert(0 <= first_target[kind] && first_target[kind] <= total[kind]);
        stride[kind] = state_count;
        const std::size_t radix = static_cast<std::size_t>(first_target[kind]) + 1;
        assert(state_count <= std::numeric_limits<std::size_t>::max() / radix);
        state_count *= radix;
    }

    std::vector<std::array<int, Categories>> first_count(state_count);
    for (std::size_t state = 0; state < state_count; ++state) {
        for (std::size_t kind = 0; kind < Categories; ++kind) {
            first_count[state][kind] = static_cast<int>(
                state / stride[kind] % (static_cast<std::size_t>(first_target[kind]) + 1)
            );
        }
    }

    std::vector<T> previous(state_count, infinity), next(state_count, infinity);
    previous[0] = T{};
    std::array<int, Categories> seen{};
    for (int index = 0; index < static_cast<int>(category.size()); ++index) {
        std::fill(next.begin(), next.end(), infinity);
        const int current_kind = category[index];
        for (std::size_t state = 0; state < state_count; ++state) {
            if (previous[state] == infinity) continue;
            std::array<int, Categories> second_count{};
            bool valid = true;
            for (std::size_t kind = 0; kind < Categories; ++kind) {
                second_count[kind] = seen[kind] - first_count[state][kind];
                if (second_count[kind] < 0) valid = false;
            }
            if (!valid) continue;

            if (first_count[state][current_kind] < first_target[current_kind]) {
                const std::size_t destination = state + stride[current_kind];
                const T candidate = previous[state] + std::invoke(
                    cost, index, 0, first_count[state], second_count
                );
                if (candidate < next[destination]) next[destination] = candidate;
            }
            if (second_count[current_kind] < total[current_kind] - first_target[current_kind]) {
                const T candidate = previous[state] + std::invoke(
                    cost, index, 1, first_count[state], second_count
                );
                if (candidate < next[state]) next[state] = candidate;
            }
        }
        ++seen[current_kind];
        previous.swap(next);
    }
    return previous.back();
}

}
