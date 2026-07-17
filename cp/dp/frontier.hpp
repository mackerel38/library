#pragma once
#include <bits/stdc++.h>

namespace poe {

/// frontierdp<State, Value> dp(initial): 同じ状態の値を+=でまとめる疎なfrontier DP。
template<class State, class Value>
struct frontierdp {
    using container_type = std::map<State, Value>;

    /// O(1)。frontierdp(): 状態を持たないDPを作る。
    frontierdp() = default;

    /// O(log states)。frontierdp(initial, value): 初期状態を一つ持つDPを作る。
    explicit frontierdp(State initial, Value value = Value{1}) {
        add(std::move(initial), value);
    }

    /// O(log states)。dp.add(state, value): stateの値へvalueを加える。
    void add(const State& state, const Value& value) { states[state] += value; }

    /// O(log states)。dp.add(move(state), value): stateを移動して値へvalueを加える。
    void add(State&& state, const Value& value) { states[std::move(state)] += value; }

    /// 遷移回数をTとしてO(T log T)。dp.step(f): f(state, value, next)で次の層を作る。
    template<class Transition>
    void step(Transition transition) {
        frontierdp next;
        for (const auto& [state, value] : states) transition(state, value, next);
        states = std::move(next.states);
    }

    /// O(states)。dp.sum(pred): pred(state)を満たす値の総和を返す。
    template<class Predicate>
    Value sum(Predicate pred) const {
        Value result{};
        for (const auto& [state, value] : states) {
            if (pred(state)) result += value;
        }
        return result;
    }

    /// O(1)。dp.size(): 現在の状態数を返す。
    int size() const noexcept { return static_cast<int>(states.size()); }

    /// O(1)。dp.empty(): 状態がなければtrueを返す。
    bool empty() const noexcept { return states.empty(); }

    container_type states;
};

}
