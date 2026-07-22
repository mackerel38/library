#include "math/gaussian.hpp"
#include "math/modint.hpp"
#include "math/randomwalk.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260716);
    using mint = modint998244353;
    const mint half = mint{1} / 2;
    for (int trial = 0; trial < 1000; ++trial) {
        const int size = 1 + random() % 8;
        vector<vector<mint>> transition(size);
        vector<mint> reward(size);
        matrix<mint> coefficients(size + 1, size + 1);
        vector<mint> right(size + 1);
        coefficients[0][0] = 1;
        for (int state = 0; state < size; ++state) {
            transition[state].resize(state + 2);
            mint sum = 0;
            for (mint& probability : transition[state]) {
                probability = 1 + random() % 20;
                sum += probability;
            }
            for (mint& probability : transition[state]) probability /= sum;
            reward[state] = random() % 20;
            coefficients[state + 1][state] += 1;
            for (int next = 0; next <= state + 1; ++next) {
                coefficients[state + 1][next] -= transition[state][next];
            }
            right[state + 1] = reward[state];
        }
        const auto expected = solve_linear(coefficients, right);
        assert(expected.status == linearstatus::unique);
        assert(upward_skipfree_potential(transition, reward) == expected.solution);
    }
    for (int people = 1; people <= 4; ++people) {
        for (int trial = 0; trial < 100; ++trial) {
            vector<int> positions(people);
            for (int& position : positions) position = 2 * static_cast<int>(random() % 5);
            constexpr int maximum_time = 7;
            const auto actual = symmetric_walk_meeting_probabilities<998244353>(
                positions, maximum_time
            );
            vector<map<int, mint>> distribution(people);
            for (int person = 0; person < people; ++person) {
                distribution[person][positions[person]] = 1;
            }
            for (int time = 0; time <= maximum_time; ++time) {
                mint expected = 0;
                for (int position = -maximum_time; position <= 8 + maximum_time; ++position) {
                    mint probability = 1;
                    for (int person = 0; person < people; ++person) {
                        probability *= distribution[person][position];
                    }
                    expected += probability;
                }
                assert(actual[time] == expected);
                for (auto& current : distribution) {
                    map<int, mint> next;
                    for (const auto& [position, probability] : current) {
                        next[position - 1] += probability * half;
                        next[position + 1] += probability * half;
                    }
                    current.swap(next);
                }
            }
        }
    }
    for (int period = 3; period <= 15; ++period) {
        for (int trial = 0; trial < 50; ++trial) {
            const int maximum_step = 1 + random() % (period - 1);
            vector<mint> probability(maximum_step);
            mint sum = 0;
            for (mint& value : probability) {
                value = 1 + random() % 20;
                sum += value;
            }
            for (mint& value : probability) value /= sum;

            matrix<mint> coefficients(period - 1, period - 1);
            vector<mint> right(period - 1, 1);
            for (int remaining = 1; remaining < period; ++remaining) {
                coefficients[remaining - 1][remaining - 1] = 1;
                for (int step = 1; step <= maximum_step; ++step) {
                    int next = remaining - step;
                    if (next < 0) next += period;
                    if (next != 0) {
                        coefficients[remaining - 1][next - 1] -= probability[step - 1];
                    }
                }
            }
            const auto expected = solve_linear(coefficients, right);
            if (expected.status != linearstatus::unique) continue;
            for (int remaining = 1; remaining < period; ++remaining) {
                assert(cyclic_hitting_expectation(period, remaining, probability)
                       == expected[remaining - 1]);
            }
        }
    }
}
