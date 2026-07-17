#include <cassert>
#include <random>
#include <vector>
#include "math/gaussian.hpp"

using namespace std;
using namespace poe;

bool satisfies(const matrix<int>& coefficients, const vector<int>& right,
               const vector<int>& values) {
    for (int row = 0; row < coefficients.height(); ++row) {
        int sum = 0;
        for (int column = 0; column < coefficients.width(); ++column) {
            sum ^= coefficients[row][column] & values[column];
        }
        if (sum != right[row]) return false;
    }
    return true;
}

int main() {
    mt19937 random(20260715);
    for (int equations = 0; equations <= 8; ++equations) {
        for (int variables = 0; variables <= 8; ++variables) {
            for (int repetition = 0; repetition < 100; ++repetition) {
                matrix<int> coefficients(equations, variables);
                vector<int> right(equations);
                for (int row = 0; row < equations; ++row) {
                    right[row] = random() & 1;
                    for (int column = 0; column < variables; ++column) {
                        coefficients[row][column] = random() & 1;
                    }
                }
                int solution_count = 0;
                for (int mask = 0; mask < (1 << variables); ++mask) {
                    vector<int> values(variables);
                    for (int column = 0; column < variables; ++column) {
                        values[column] = mask >> column & 1;
                    }
                    solution_count += satisfies(coefficients, right, values);
                }

                const auto result = solve_binary_linear(coefficients, right);
                assert(result.rank == binary_matrix_rank(coefficients));
                if (solution_count == 0) {
                    assert(result.status == linearstatus::none);
                    continue;
                }
                assert(satisfies(coefficients, right, result.solution));
                assert(solution_count == (1 << result.basis.size()));
                for (const auto& direction : result.basis) {
                    assert(satisfies(coefficients, vector<int>(equations), direction));
                }
                assert((result.status == linearstatus::unique) == result.basis.empty());
            }
        }
    }
}
