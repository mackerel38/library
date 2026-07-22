#include "math/qanalog.hpp"

template <int field_size>
void test_combination() {
    using mint = poe::staticmodint<998244353>;
    constexpr int limit = 20;
    const poe::qcombination<mint> combination(limit, mint{field_size});
    std::vector<std::vector<mint>> expected(limit + 1,
                                            std::vector<mint>(limit + 1));
    expected[0][0] = 1;
    for (int n = 1; n <= limit; ++n) {
        mint power = 1;
        for (int k = 0; k <= n; ++k) {
            expected[n][k] = (k > 0 ? expected[n - 1][k - 1] : mint{}) +
                             power * expected[n - 1][k];
            power *= field_size;
        }
    }
    for (int n = 0; n <= limit; ++n) {
        for (int k = 0; k <= n; ++k) {
            assert(combination.choose(n, k) == expected[n][k]);
        }
    }
}

long long brute_avoiding(int rows, int dimension, int field_size) {
    long long matrices = 1;
    for (int cell = 0; cell < rows * dimension; ++cell) matrices *= field_size;
    long long result = 0;
    std::vector<int> matrix(rows * dimension);
    for (long long code = 0; code < matrices; ++code) {
        long long current = code;
        for (int& value : matrix) {
            value = current % field_size;
            current /= field_size;
        }
        bool contains = false;
        long long coefficients = 1;
        for (int row = 0; row < rows; ++row) coefficients *= field_size;
        for (long long choice = 0; choice < coefficients && !contains; ++choice) {
            long long remaining = choice;
            std::vector<int> sum(dimension);
            for (int row = 0; row < rows; ++row) {
                const int coefficient = remaining % field_size;
                remaining /= field_size;
                for (int column = 0; column < dimension; ++column) {
                    sum[column] = (sum[column] +
                                   coefficient * matrix[row * dimension + column]) %
                                  field_size;
                }
            }
            contains = sum[0] == 1;
            for (int column = 1; column < dimension; ++column) {
                contains = contains && sum[column] == 0;
            }
        }
        result += !contains;
    }
    return result;
}

int main() {
    test_combination<2>();
    test_combination<3>();
    for (int field_size : {2, 3}) {
        for (int dimension = 1; dimension <= 3; ++dimension) {
            const int maximum_rows = 3;
            const auto actual =
                field_size == 2
                    ? poe::matrix_rowspace_avoiding_vector_counts<998244353>(
                          maximum_rows, dimension, 2)
                    : poe::matrix_rowspace_avoiding_vector_counts<998244353>(
                          maximum_rows, dimension, 3);
            for (int rows = 0; rows <= maximum_rows; ++rows) {
                assert(actual[rows] ==
                       brute_avoiding(rows, dimension, field_size));
            }
        }
    }
}
