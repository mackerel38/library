#pragma once
#include <bits/stdc++.h>
#include "math/matrix.hpp"
#include "string/trie.hpp"

namespace poe {

/// 禁止patternを含まない長さlengthの語数を数える。O(states^3 log length + states sigma log sigma)。
template <class T, std::ranges::input_range Patterns, std::integral Symbol>
T count_avoiding_words(long long length, const Patterns& patterns, Symbol first, Symbol last) {
    assert(length >= 0 && first <= last);
    ahocorasick<Symbol> automaton(first, last);
    for (const auto& pattern : patterns) automaton.add(pattern);
    automaton.build();

    const int states = automaton.size();
    std::vector<char> forbidden(states);
    for (int state = 0; state < states; ++state) {
        forbidden[state] = !automaton.patterns(state).empty();
    }
    matrix<T> transition(states, states);
    for (int state = 0; state < states; ++state) {
        if (forbidden[state]) continue;
        for (long long raw = static_cast<long long>(first);
             raw <= static_cast<long long>(last); ++raw) {
            const int next = automaton.next(state, static_cast<Symbol>(raw));
            if (!forbidden[next]) transition[state][next] += T{1};
        }
    }
    const auto powered = transition.pow(length);
    T answer{};
    for (int state = 0; state < states; ++state) answer += powered[0][state];
    return answer;
}

}
