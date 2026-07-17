#include "math/gaussian.hpp"
#include "math/modint.hpp"
#include "math/randomwalk.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260716);
    using mint = modint998244353;
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
