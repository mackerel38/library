#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(V+E)。暗黙automatonで最短、同長なら辞書順最小の受理語を返す。
template <class Symbol = char, class Accept, class Expand>
std::optional<std::vector<Symbol>> shortest_lexicographic_word(
    int state_count,
    int start,
    Accept accept,
    Expand expand
) {
    assert(state_count >= 0 && 0 <= start && start < state_count);
    if (accept(start)) return std::vector<Symbol>{};
    std::vector<int> parent(state_count, -2);
    std::vector<Symbol> parent_symbol(state_count);
    std::vector<int> queue;
    queue.reserve(state_count);
    parent[start] = -1;
    queue.push_back(start);
    for (std::size_t head = 0; head < queue.size(); ++head) {
        const int state = queue[head];
        int goal = -1;
        auto relax = [&](int next, Symbol symbol) {
            assert(0 <= next && next < state_count);
            if (parent[next] != -2) return false;
            parent[next] = state;
            parent_symbol[next] = symbol;
            queue.push_back(next);
            if (accept(next) && goal == -1) goal = next;
            return true;
        };
        expand(state, relax);
        if (goal == -1) continue;
        std::vector<Symbol> word;
        while (goal != start) {
            word.push_back(parent_symbol[goal]);
            goal = parent[goal];
        }
        std::ranges::reverse(word);
        return word;
    }
    return std::nullopt;
}

}
