#pragma once
#include <bits/stdc++.h>
#include "math/gaussian.hpp"
#include "math/matrix.hpp"

namespace poe {

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
