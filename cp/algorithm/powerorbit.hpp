#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 有限monoidのbase^k列: finitepowerorbit orbit(one, base, multiply); 構築O(s log s)、参照O(1)。
template <class State>
struct finitepowerorbit {
    /// O(s log s)。初めて同じ状態へ戻るまで冪を列挙し、前周期と周期を求める。
    template <class Multiply>
    finitepowerorbit(State one, const State& base, Multiply multiply) {
        std::map<State, int> first;
        State current = std::move(one);
        while (!first.contains(current)) {
            first[current] = static_cast<int>(values_.size());
            values_.push_back(current);
            current = multiply(current, base);
        }
        preperiod_ = first[current];
        period_ = static_cast<int>(values_.size()) - preperiod_;
    }

    /// O(1)。base^exponentを返す。
    const State& operator[](unsigned long long exponent) const {
        if (exponent < values_.size()) return values_[static_cast<std::size_t>(exponent)];
        const auto index = preperiod_ +
            static_cast<int>((exponent - preperiod_) % static_cast<unsigned long long>(period_));
        return values_[index];
    }

    /// O(1)。周期へ入る最初の指数を返す。
    int preperiod() const noexcept { return preperiod_; }

    /// O(1)。周期の長さを返す。
    int period() const noexcept { return period_; }

    /// O(1)。保持している相異なる状態数を返す。
    int size() const noexcept { return static_cast<int>(values_.size()); }

private:
    std::vector<State> values_;
    int preperiod_ = 0;
    int period_ = 0;
};

}
