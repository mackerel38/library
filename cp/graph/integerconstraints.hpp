#pragma once
#include <bits/stdc++.h>
#include "graph/twosat.hpp"

namespace poe {

/// 有界整数変数へ二変数和の上下限制約を追加する: boundedsumsat sat(bounds)。
struct boundedsumsat {
private:
    struct literal {
        int variable = -1;
        bool value = false;
        int constant = -1;
    };

    static std::vector<int> make_offsets(
        const std::vector<std::pair<int, int>>& bounds
    ) {
        std::vector<int> offsets(static_cast<std::size_t>(bounds.size()) + 1);
        for (int index = 0; index < static_cast<int>(bounds.size()); ++index) {
            assert(bounds[index].first <= bounds[index].second);
            const long long width = static_cast<long long>(bounds[index].second) -
                                    bounds[index].first;
            assert(width <= std::numeric_limits<int>::max() - offsets[index]);
            offsets[index + 1] = offsets[index] + static_cast<int>(width);
        }
        return offsets;
    }

    static std::vector<std::pair<int, int>> make_uniform_bounds(
        int variable_count, int maximum_value
    ) {
        assert(variable_count >= 0 && maximum_value >= 0);
        return std::vector<std::pair<int, int>>(
            static_cast<std::size_t>(variable_count), {0, maximum_value});
    }

    void narrower_first(int& i, int& j) const {
        const auto width = [&](int variable) {
            return static_cast<long long>(bounds_[variable].second) -
                   bounds_[variable].first;
        };
        if (width(i) > width(j)) std::swap(i, j);
    }

    static long long shifted_bound(long long bound, long long threshold) {
        const long long delta = 1 - threshold;
        if (delta > 0 && bound > std::numeric_limits<long long>::max() - delta) {
            return std::numeric_limits<long long>::max();
        }
        if (delta < 0 && bound < std::numeric_limits<long long>::min() - delta) {
            return std::numeric_limits<long long>::min();
        }
        return bound + delta;
    }

    void add_sum_lower_impl(int i, int j, long long lower) {
        narrower_first(i, j);
        const auto [minimum, maximum] = bounds_[i];
        for (long long threshold = static_cast<long long>(minimum) + 1;
             threshold <= static_cast<long long>(maximum) + 1; ++threshold) {
            add_or(greater_equal(i, threshold),
                   greater_equal(j, shifted_bound(lower, threshold)));
        }
    }

    void add_sum_upper_impl(int i, int j, long long upper) {
        narrower_first(i, j);
        const auto [minimum, maximum] = bounds_[i];
        for (long long threshold = minimum; threshold <= maximum; ++threshold) {
            add_or(negate(greater_equal(i, threshold)),
                   negate(greater_equal(j, shifted_bound(upper, threshold))));
        }
    }

    literal greater_equal(int variable, long long threshold) const {
        const auto [lower, upper] = bounds_[variable];
        if (threshold <= lower) return literal{.constant = 1};
        if (threshold > upper) return literal{.constant = 0};
        return literal{offsets_[variable] +
                           static_cast<int>(threshold - lower - 1),
                       true, -1};
    }

    static literal negate(literal value) {
        if (value.constant >= 0) {
            value.constant ^= 1;
        } else {
            value.value = !value.value;
        }
        return value;
    }

    void add_or(literal left, literal right) {
        if (left.constant == 1 || right.constant == 1) return;
        if (left.constant == 0 && right.constant == 0) {
            impossible_ = true;
            return;
        }
        if (left.constant == 0) {
            sat_.force(right.variable, right.value);
        } else if (right.constant == 0) {
            sat_.force(left.variable, left.value);
        } else {
            sat_.add_clause(left.variable, left.value, right.variable, right.value);
        }
    }

public:
    /// O(sum(upper-lower))。各変数iの定義域を整数閉区間bounds[i]にする。
    explicit boundedsumsat(std::vector<std::pair<int, int>> bounds)
        : bounds_(std::move(bounds)), offsets_(make_offsets(bounds_)),
          sat_(offsets_.back()), answer_(bounds_.size()) {
        for (int variable = 0; variable < variables(); ++variable) {
            const auto [lower, upper] = bounds_[variable];
            for (long long threshold = static_cast<long long>(lower) + 2;
                 threshold <= upper; ++threshold) {
                const auto high = greater_equal(variable, threshold);
                const auto low = greater_equal(variable, threshold - 1);
                sat_.imply(high.variable, high.value, low.variable, low.value);
            }
        }
    }

    /// O(nM)。n変数をすべて整数閉区間[0,maximum_value]にする。
    boundedsumsat(int variable_count, int maximum_value)
        : boundedsumsat(make_uniform_bounds(variable_count, maximum_value)) {}

    /// O(min(width_i,width_j))。lower<=x_i+x_j<=upperを追加する。
    void add_sum_constraint(int i, int j, long long lower, long long upper) {
        assert(0 <= i && i < variables() && 0 <= j && j < variables());
        has_answer_ = false;
        if (lower > upper) {
            impossible_ = true;
            return;
        }
        add_sum_lower_impl(i, j, lower);
        add_sum_upper_impl(i, j, upper);
    }

    /// O(min(width_i,width_j))。x_i+x_j>=lowerを追加する。
    void add_sum_lower(int i, int j, long long lower) {
        assert(0 <= i && i < variables() && 0 <= j && j < variables());
        has_answer_ = false;
        add_sum_lower_impl(i, j, lower);
    }

    /// O(min(width_i,width_j))。x_i+x_j<=upperを追加する。
    void add_sum_upper(int i, int j, long long upper) {
        assert(0 <= i && i < variables() && 0 <= j && j < variables());
        has_answer_ = false;
        add_sum_upper_impl(i, j, upper);
    }

    /// O(V+E)。充足可能ならtrueを返し、整数割当てanswer()を構築する。
    bool satisfiable() {
        has_answer_ = false;
        if (impossible_ || !sat_.satisfiable()) return false;
        for (int variable = 0; variable < variables(); ++variable) {
            answer_[variable] = bounds_[variable].first;
            for (int node = offsets_[variable]; node < offsets_[variable + 1]; ++node) {
                if (sat_[node]) {
                    answer_[variable] = bounds_[variable].first +
                                        (node - offsets_[variable]) + 1;
                }
            }
        }
        has_answer_ = true;
        return true;
    }

    /// O(1)。直前のsatisfiable()で得た整数割当てを返す。
    const std::vector<int>& answer() const {
        assert(has_answer_);
        return answer_;
    }

    /// O(1)。直前に得た変数indexの整数値を返す。
    int operator[](int index) const {
        assert(0 <= index && index < variables() && has_answer_);
        return answer_[index];
    }

    /// O(1)。整数変数の個数を返す。
    int variables() const noexcept { return static_cast<int>(bounds_.size()); }

private:
    std::vector<std::pair<int, int>> bounds_;
    std::vector<int> offsets_;
    twosat sat_;
    std::vector<int> answer_;
    bool impossible_ = false;
    bool has_answer_ = false;
};

}
