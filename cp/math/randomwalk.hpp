#pragma once
#include <bits/stdc++.h>
#include "math/convolution.hpp"
#include "math/gaussian.hpp"
#include "math/matrix.hpp"

namespace poe {

/// O(limit^(3/4))時間・O(sqrt(limit))領域。一様な1..facesを掛け、limitを超えるまでの期待回数を返す。
template<class T>
T uniform_multiplicative_stopping_expectation(long long faces, long long limit) {
    assert(faces >= 2);
    assert(limit >= 0);
    if (limit == 0) return T{};
    long long root = std::sqrt(static_cast<long double>(limit));
    while ((root + 1) <= limit / (root + 1)) ++root;
    while (root > limit / root) --root;
    std::vector<T> low(root + 1), high(root + 1);
    std::vector<char> low_seen(root + 1), high_seen(root + 1);
    auto solve = [&](auto&& self, long long maximum_product) -> T {
        if (maximum_product == 0) return T{};
        const bool is_low = maximum_product <= root;
        const long long index = is_low ? maximum_product : limit / maximum_product;
        T& memo = is_low ? low[index] : high[index];
        char& seen = is_low ? low_seen[index] : high_seen[index];
        if (seen) return memo;
        T sum{};
        const long long maximum_face = std::min(faces, maximum_product);
        for (long long left = 2; left <= maximum_face;) {
            const long long quotient = maximum_product / left;
            const long long right = std::min(maximum_face, maximum_product / quotient);
            sum += T{right - left + 1} * self(self, quotient);
            left = right + 1;
        }
        memo = (T{faces} + sum) / T{faces - 1};
        seen = true;
        return memo;
    };
    return solve(solve, limit);
}

/// O(n^2)時間・O(n)追加領域。上方向へ高々1だけ進むMarkov連鎖のh[i]=reward[i]+E[h[next]]をh[0]=0から解く。
template<class T>
std::vector<T> upward_skipfree_potential(
    const std::vector<std::vector<T>>& transition,
    const std::vector<T>& reward
) {
    const int size = static_cast<int>(transition.size());
    assert(static_cast<int>(reward.size()) == size);
    std::vector<T> potential(size + 1);
    for (int state = 0; state < size; ++state) {
        assert(static_cast<int>(transition[state].size()) == state + 2);
        assert(transition[state][state + 1] != T{});
        T known = reward[state];
        for (int next = 0; next <= state; ++next) {
            known += transition[state][next] * potential[next];
        }
        potential[state + 1] = (potential[state] - known) / transition[state][state + 1];
    }
    return potential;
}

/// O(pn+n log p+M(n))。同parityの各位置から独立な±1 walkを始め、時刻0..nに全員が同一点にいる確率を返す。
template<int mod>
std::vector<staticmodint<mod>> symmetric_walk_meeting_probabilities(
    const std::vector<int>& positions,
    int maximum_time
) {
    using mint = staticmodint<mod>;
    assert(!positions.empty() && maximum_time >= 0);
    const int minimum = *std::ranges::min_element(positions);
    std::vector<int> normalized;
    normalized.reserve(positions.size());
    int maximum_position = 0;
    for (const int position : positions) {
        assert((position - minimum) % 2 == 0);
        const int value = (position - minimum) / 2;
        normalized.push_back(value);
        maximum_position = std::max(maximum_position, value);
    }

    const int factorial_limit = maximum_time + maximum_position;
    assert(factorial_limit < mod);
    std::vector<mint> factorial(factorial_limit + 1, mint{1});
    std::vector<mint> inverse_factorial(factorial_limit + 1, mint{1});
    for (int value = 1; value <= factorial_limit; ++value) {
        factorial[value] = factorial[value - 1] * value;
    }
    inverse_factorial[factorial_limit] = factorial[factorial_limit].inv();
    for (int value = factorial_limit; value > 0; --value) {
        inverse_factorial[value - 1] = inverse_factorial[value] * value;
    }

    std::vector<mint> forward(maximum_time + 1, mint{1});
    std::vector<mint> backward(maximum_time + 1, mint{1});
    for (const int position : normalized) {
        for (int index = 0; index <= maximum_time; ++index) {
            if (index < position) forward[index] = mint{};
            else forward[index] *= inverse_factorial[index - position];
            backward[index] *= inverse_factorial[index + position];
        }
    }
    const auto product = convolution(std::move(forward), std::move(backward));

    const long long people = static_cast<long long>(positions.size());
    const mint denominator_step = mint{2}.pow(people);
    mint scale = 1;
    std::vector<mint> result(maximum_time + 1);
    for (int time = 0; time <= maximum_time; ++time) {
        if (time > 0) scale *= mint{time}.pow(people) / denominator_step;
        result[time] = product[time] * scale;
    }
    return result;
}

/// O(d^3 log period)。正のstep和がremaining mod periodへ初到達するまでの期待step数を返す。
template<class T>
T cyclic_hitting_expectation(
    long long period,
    long long remaining,
    std::vector<T> step_probability
) {
    while (!step_probability.empty() && step_probability.back() == T{}) {
        step_probability.pop_back();
    }
    assert(!step_probability.empty());
    const int maximum_step = static_cast<int>(step_probability.size());
    assert(period > maximum_step);
    assert(1 <= remaining && remaining < period);
    T probability_sum{};
    for (const T& probability : step_probability) probability_sum += probability;
    assert(probability_sum == T{1});

    const int dimension = maximum_step + 1;
    matrix<T> transition(dimension, dimension);
    for (int step = 1; step <= maximum_step; ++step) {
        transition[0][step - 1] = step_probability[step - 1];
    }
    transition[0][maximum_step] = 1;
    for (int index = 1; index < maximum_step; ++index) {
        transition[index][index - 1] = 1;
    }
    transition[maximum_step][maximum_step] = 1;

    const matrix<T> high = transition.pow(period - maximum_step);
    const int variables = maximum_step - 1;
    matrix<T> coefficients(variables, variables);
    std::vector<T> right(variables, T{1});
    for (int x = 1; x < maximum_step; ++x) {
        coefficients[x - 1][x - 1] += 1;
        for (int step = 1; step < x; ++step) {
            coefficients[x - 1][x - step - 1] -= step_probability[step - 1];
        }
        for (int step = x + 1; step <= maximum_step; ++step) {
            const int high_row = step - x - 1;
            const T probability = step_probability[step - 1];
            for (int variable = 0; variable < variables; ++variable) {
                const int initial_column = maximum_step - 2 - variable;
                coefficients[x - 1][variable] -= probability * high[high_row][initial_column];
            }
            right[x - 1] += probability * high[high_row][maximum_step];
        }
    }

    std::vector<T> initial(dimension);
    if (variables > 0) {
        const auto solution = solve_linear(std::move(coefficients), right);
        assert(solution.status == linearstatus::unique);
        for (int value = 1; value < maximum_step; ++value) {
            initial[maximum_step - 1 - value] = solution[value - 1];
        }
    }
    initial[maximum_step] = 1;
    if (remaining < maximum_step) {
        return initial[maximum_step - 1 - remaining];
    }
    const matrix<T> advance = transition.pow(remaining - maximum_step + 1);
    T answer{};
    for (int column = 0; column < dimension; ++column) {
        answer += advance[0][column] * initial[column];
    }
    return answer;
}

}
