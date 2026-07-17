#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 桁DPで先頭の0を通常の桁として使うか、数が始まるまで無視するかを指定する。
enum class leadingzero { use, ignore };

/// digitdpの終端状態。started=falseはleadingzero::ignoreで数0だけを表す。
template<class State>
struct digitdpstate {
    State value;
    bool started;

    friend bool operator==(const digitdpstate&, const digitdpstate&) = default;
    friend auto operator<=>(const digitdpstate&, const digitdpstate&) = default;
};

/// digitdpの状態別集計結果。
template<class State, class Value>
struct digitdpresult {
    /// O(states)。result.sum(pred): pred(state, started)を満たす値の総和を返す。
    template<class Predicate>
    Value sum(Predicate pred) const {
        Value answer{};
        for (const auto& [state, value] : states) {
            if (pred(state.value, state.started)) answer += value;
        }
        return answer;
    }

    std::map<digitdpstate<State>, Value> states;
};

namespace detail {

template<class State, class Transition>
std::optional<State> digitdp_transition(Transition& transition, const State& state, int digit, int position) {
    using result_type = std::remove_cvref_t<std::invoke_result_t<Transition, const State&, int, int>>;
    if constexpr (std::same_as<result_type, State>) {
        return transition(state, digit, position);
    } else {
        return transition(state, digit, position);
    }
}

}

/// O(length * base * states * log states)。digitdp(digits, base, initial, transition): 0以上upper以下を集計する。
/// transition(state, digit, position)は次状態またはoptional<状態>を返す。
template<class Value, class State, class Transition>
digitdpresult<State, Value> digitdp(
    const std::vector<int>& digits,
    int base,
    State initial,
    Transition transition,
    leadingzero zeros = leadingzero::ignore,
    Value one = Value{1}
) {
    assert(base >= 2);
    for (const int digit : digits) assert(0 <= digit && digit < base);
    using key = std::tuple<State, bool, bool>;
    std::map<key, Value> dp;
    dp[{std::move(initial), true, zeros == leadingzero::use}] = one;
    for (int position = 0; position < static_cast<int>(digits.size()); ++position) {
        std::map<key, Value> next;
        for (const auto& [current, ways] : dp) {
            const auto& [state, tight, started] = current;
            const int limit = tight ? digits[position] : base - 1;
            for (int digit = 0; digit <= limit; ++digit) {
                const bool next_tight = tight && digit == limit;
                const bool next_started = started || digit != 0;
                if (zeros == leadingzero::ignore && !next_started) {
                    next[{state, next_tight, false}] += ways;
                    continue;
                }
                auto moved = detail::digitdp_transition<State>(transition, state, digit, position);
                if (moved) next[{std::move(*moved), next_tight, true}] += ways;
            }
        }
        dp = std::move(next);
    }
    digitdpresult<State, Value> result;
    for (const auto& [current, ways] : dp) {
        const auto& [state, tight, started] = current;
        (void)tight;
        result.states[{state, started}] += ways;
    }
    return result;
}

/// O(length * 10 * states * log states)。digitdp(upper, initial, transition): 十進数の0以上upper以下を集計する。
template<class Value, class State, class Transition>
digitdpresult<State, Value> digitdp(
    std::string_view upper,
    State initial,
    Transition transition,
    leadingzero zeros = leadingzero::ignore,
    Value one = Value{1}
) {
    assert(!upper.empty());
    std::vector<int> digits;
    digits.reserve(upper.size());
    for (const char character : upper) {
        assert('0' <= character && character <= '9');
        digits.push_back(character - '0');
    }
    return digitdp<Value>(digits, 10, std::move(initial), std::move(transition), zeros, one);
}

}
