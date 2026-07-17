#pragma once
#include <bits/stdc++.h>
#include "dp/frontier.hpp"
#include "dp/profile.hpp"

namespace poe {

/// connectivityfrontierdp<Value> dp(height, marked): markedを含む全初列から盤面連結DPを始める。
/// 各列では選択マス間の上下辺と前列への横辺をすべて張る問題だけに使う。
template<class Value>
struct connectivityfrontierdp {
    /// O(2^height * height^2 log states)。markedを必ず選ぶ全初列を列挙する。
    explicit connectivityfrontierdp(int height, int marked, Value value = Value{1})
        : height(height), marked(marked) {
        assert(0 <= height && height < 63);
        assert(0 <= marked && marked < height);
        const std::uint64_t patterns = 1ULL << height;
        for (std::uint64_t mask = 0; mask < patterns; ++mask) {
            if (mask >> marked & 1ULL) {
                dp.add(connectivityprofile::from_mask(height, mask, marked), value);
            }
        }
    }

    /// 状態数をSとしてO(S * 2^height * height^2 log S)。次の列を全列挙して進める。
    void advance_all() {
        const std::uint64_t patterns = 1ULL << height;
        dp.step([&](const connectivityprofile& state, const Value& ways, auto& next) {
            for (std::uint64_t mask = 0; mask < patterns; ++mask) {
                auto advanced = state.advance(mask);
                if (advanced.has_marked()) next.add(std::move(advanced), ways);
            }
        });
    }

    /// O(states)。dp.sum_marked(position): positionが注目成分に属する状態の総和を返す。
    Value sum_marked(int position) const {
        assert(0 <= position && position < height);
        return dp.sum([&](const connectivityprofile& state) { return state.is_marked(position); });
    }

    int height;
    int marked;
    frontierdp<connectivityprofile, Value> dp;
};

/// O(rows*S*2^width*width^2 log S)。必須マスを含む最小追加マス数の連結supersetを返す。
inline int minimum_connected_grid_completion(
    std::vector<std::uint64_t> required,
    int width
) {
    assert(0 <= width && width < 63);
    const std::uint64_t patterns = std::uint64_t{1} << width;
    for (const std::uint64_t mask : required) assert(mask < patterns);
    int first = 0;
    while (first < static_cast<int>(required.size()) && required[first] == 0) ++first;
    assert(first < static_cast<int>(required.size()));
    int last = static_cast<int>(required.size()) - 1;
    while (required[last] == 0) --last;
    required = std::vector<std::uint64_t>(required.begin() + first, required.begin() + last + 1);

    constexpr int infinity = std::numeric_limits<int>::max() / 4;
    std::map<connectivityprofile, int> states;
    for (std::uint64_t mask = 0; mask < patterns; ++mask) {
        if ((mask & required[0]) != required[0]) continue;
        states[connectivityprofile::from_mask(width, mask)] =
            std::popcount(mask ^ required[0]);
    }
    for (int row = 1; row < static_cast<int>(required.size()); ++row) {
        std::map<connectivityprofile, int> next;
        for (const auto& [state, cost] : states) {
            for (std::uint64_t mask = 0; mask < patterns; ++mask) {
                if ((mask & required[row]) != required[row]) continue;
                bool retained = true;
                for (int position = 0; position < width; ++position) {
                    if (!state.occupied(position)) continue;
                    bool first_label = true;
                    for (int previous = 0; previous < position; ++previous) {
                        if (state[previous] == state[position]) first_label = false;
                    }
                    if (!first_label) continue;
                    bool survives = false;
                    for (int current = 0; current < width; ++current) {
                        if (state[current] == state[position] && (mask >> current & 1ULL)) {
                            survives = true;
                        }
                    }
                    if (!survives) {
                        retained = false;
                        break;
                    }
                }
                if (!retained) continue;
                auto advanced = state.advance(mask);
                const int candidate = cost + std::popcount(mask ^ required[row]);
                auto [iterator, inserted] = next.emplace(std::move(advanced), candidate);
                if (!inserted) iterator->second = std::min(iterator->second, candidate);
            }
        }
        states = std::move(next);
    }
    int answer = infinity;
    for (const auto& [state, cost] : states) {
        int component = -1;
        bool connected = true;
        for (int position = 0; position < width; ++position) {
            if (!state.occupied(position)) continue;
            if (component == -1) component = state[position];
            else if (component != state[position]) connected = false;
        }
        if (connected) answer = std::min(answer, cost);
    }
    assert(answer != infinity);
    return answer;
}

}
