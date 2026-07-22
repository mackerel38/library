#pragma once
#include <bits/stdc++.h>
#include "math/matrix.hpp"

namespace poe {

/// linearstatus: 連立一次方程式の解なし・一意解・複数解。
enum class linearstatus { none, unique, multiple };

/// solve_linearの結果。solutionは特殊解、basisは斉次解空間の基底。
template<class T>
struct linearsystemresult {
    linearstatus status = linearstatus::none;
    int rank = 0;
    std::vector<T> solution;
    std::vector<std::vector<T>> basis;

    /// O(1)。result[i]: 特殊解のi番目を返す。
    const T& operator[](int index) const { return solution[index]; }
};

/// O(h*w*min(h,w))。matrix_rank(a): 体上の行列aのrankを返す。
template<class T>
int matrix_rank(matrix<T> values) {
    int rank = 0;
    for (int column = 0; column < values.width() && rank < values.height(); ++column) {
        int pivot = rank;
        while (pivot < values.height() && values[pivot][column] == T{}) ++pivot;
        if (pivot == values.height()) continue;
        std::swap(values[pivot], values[rank]);
        const T inverse = T{1} / values[rank][column];
        for (int j = column; j < values.width(); ++j) values[rank][j] *= inverse;
        for (int row = rank + 1; row < values.height(); ++row) {
            if (values[row][column] == T{}) continue;
            const T factor = values[row][column];
            for (int j = column; j < values.width(); ++j) values[row][j] -= factor * values[rank][j];
        }
        ++rank;
    }
    return rank;
}

/// O(n^3)。determinant(a): 体上の正方行列aの行列式を返す。
template<class T>
T determinant(matrix<T> values) {
    assert(values.height() == values.width());
    T result{1};
    for (int column = 0; column < values.width(); ++column) {
        int pivot = column;
        while (pivot < values.height() && values[pivot][column] == T{}) ++pivot;
        if (pivot == values.height()) return T{};
        if (pivot != column) {
            std::swap(values[pivot], values[column]);
            result = -result;
        }
        const T pivot_value = values[column][column];
        result *= pivot_value;
        for (int row = column + 1; row < values.height(); ++row) {
            if (values[row][column] == T{}) continue;
            const T factor = values[row][column] / pivot_value;
            for (int j = column; j < values.width(); ++j) values[row][j] -= factor * values[column][j];
        }
    }
    return result;
}

/// O(h*w*min(h,w))。solve_linear(a,b): 体上のAx=bを解き、特殊解と斉次解基底を返す。
template<class T>
linearsystemresult<T> solve_linear(matrix<T> coefficients, const std::vector<T>& right) {
    assert(coefficients.height() == static_cast<int>(right.size()));
    const int equations = coefficients.height();
    const int variables = coefficients.width();
    matrix<T> augmented(equations, variables + 1);
    for (int row = 0; row < equations; ++row) {
        for (int column = 0; column < variables; ++column) augmented[row][column] = coefficients[row][column];
        augmented[row][variables] = right[row];
    }
    std::vector<int> pivot_row(variables, -1);
    int rank = 0;
    for (int column = 0; column < variables && rank < equations; ++column) {
        int pivot = rank;
        while (pivot < equations && augmented[pivot][column] == T{}) ++pivot;
        if (pivot == equations) continue;
        std::swap(augmented[pivot], augmented[rank]);
        const T inverse = T{1} / augmented[rank][column];
        for (int j = column; j <= variables; ++j) augmented[rank][j] *= inverse;
        for (int row = 0; row < equations; ++row) {
            if (row == rank || augmented[row][column] == T{}) continue;
            const T factor = augmented[row][column];
            for (int j = column; j <= variables; ++j) augmented[row][j] -= factor * augmented[rank][j];
        }
        pivot_row[column] = rank++;
    }
    for (int row = rank; row < equations; ++row) {
        bool all_zero = true;
        for (int column = 0; column < variables; ++column) all_zero &= augmented[row][column] == T{};
        if (all_zero && augmented[row][variables] != T{}) return {linearstatus::none, rank, {}, {}};
    }
    linearsystemresult<T> result;
    result.rank = rank;
    result.solution.assign(variables, T{});
    for (int column = 0; column < variables; ++column) {
        if (pivot_row[column] != -1) result.solution[column] = augmented[pivot_row[column]][variables];
    }
    for (int free = 0; free < variables; ++free) {
        if (pivot_row[free] != -1) continue;
        std::vector<T> direction(variables);
        direction[free] = T{1};
        for (int pivot = 0; pivot < variables; ++pivot) {
            if (pivot_row[pivot] != -1) direction[pivot] = -augmented[pivot_row[pivot]][free];
        }
        result.basis.push_back(std::move(direction));
    }
    result.status = result.basis.empty() ? linearstatus::unique : linearstatus::multiple;
    return result;
}

/// O(hw min(h,w)/64)。0/1行列aをGF(2)上でbit並列Gauss消去し、rankを返す。
inline int binary_matrix_rank(const matrix<int>& values) {
    const int rows = values.height();
    const int columns = values.width();
    const int words = (columns + 63) / 64;
    std::vector<std::vector<std::uint64_t>> packed(
        rows, std::vector<std::uint64_t>(words));
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            assert(values[row][column] == 0 || values[row][column] == 1);
            if (values[row][column]) packed[row][column >> 6] |= std::uint64_t{1} << (column & 63);
        }
    }
    int rank = 0;
    for (int column = 0; column < columns && rank < rows; ++column) {
        int pivot = rank;
        while (pivot < rows && ((packed[pivot][column >> 6] >> (column & 63)) & 1U) == 0) {
            ++pivot;
        }
        if (pivot == rows) continue;
        std::swap(packed[pivot], packed[rank]);
        for (int row = rank + 1; row < rows; ++row) {
            if (((packed[row][column >> 6] >> (column & 63)) & 1U) == 0) continue;
            for (int word = column >> 6; word < words; ++word) {
                packed[row][word] ^= packed[rank][word];
            }
        }
        ++rank;
    }
    return rank;
}

/// O(hw min(h,w)/64 + w^2)。GF(2)上のAx=bを解き、0/1の特殊解と斉次解基底を返す。
inline linearsystemresult<int> solve_binary_linear(
    const matrix<int>& coefficients,
    const std::vector<int>& right
) {
    assert(coefficients.height() == static_cast<int>(right.size()));
    const int equations = coefficients.height();
    const int variables = coefficients.width();
    const int words = (variables + 1 + 63) / 64;
    std::vector<std::vector<std::uint64_t>> rows(
        equations, std::vector<std::uint64_t>(words));
    for (int row = 0; row < equations; ++row) {
        assert(right[row] == 0 || right[row] == 1);
        for (int column = 0; column < variables; ++column) {
            assert(coefficients[row][column] == 0 || coefficients[row][column] == 1);
            if (coefficients[row][column]) {
                rows[row][column >> 6] |= std::uint64_t{1} << (column & 63);
            }
        }
        if (right[row]) rows[row][variables >> 6] |= std::uint64_t{1} << (variables & 63);
    }

    std::vector<int> pivot_row(variables, -1);
    int rank = 0;
    for (int column = 0; column < variables && rank < equations; ++column) {
        int pivot = rank;
        while (pivot < equations && ((rows[pivot][column >> 6] >> (column & 63)) & 1U) == 0) {
            ++pivot;
        }
        if (pivot == equations) continue;
        std::swap(rows[pivot], rows[rank]);
        for (int row = 0; row < equations; ++row) {
            if (row == rank || ((rows[row][column >> 6] >> (column & 63)) & 1U) == 0) continue;
            for (int word = column >> 6; word < words; ++word) {
                rows[row][word] ^= rows[rank][word];
            }
        }
        pivot_row[column] = rank++;
    }

    for (int row = rank; row < equations; ++row) {
        if ((rows[row][variables >> 6] >> (variables & 63)) & 1U) {
            return {linearstatus::none, rank, {}, {}};
        }
    }

    linearsystemresult<int> result;
    result.rank = rank;
    result.solution.assign(variables, 0);
    for (int column = 0; column < variables; ++column) {
        if (pivot_row[column] != -1) {
            result.solution[column] =
                (rows[pivot_row[column]][variables >> 6] >> (variables & 63)) & 1U;
        }
    }
    for (int free = 0; free < variables; ++free) {
        if (pivot_row[free] != -1) continue;
        std::vector<int> direction(variables);
        direction[free] = 1;
        for (int pivot = 0; pivot < variables; ++pivot) {
            if (pivot_row[pivot] != -1) {
                direction[pivot] =
                    (rows[pivot_row[pivot]][free >> 6] >> (free & 63)) & 1U;
            }
        }
        result.basis.push_back(std::move(direction));
    }
    result.status = result.basis.empty() ? linearstatus::unique : linearstatus::multiple;
    return result;
}

/// O(mn(min(m,n)/64+1))。GF(2)上の疎な方程式から特殊解を一つ返す。解なしならnullopt。
inline std::optional<std::vector<int>> solve_sparse_binary_linear_one(
    int variables,
    const std::vector<std::vector<int>>& nonzero_columns,
    const std::vector<int>& right
) {
    assert(variables >= 0 && nonzero_columns.size() == right.size());
    const int equations = static_cast<int>(right.size());
    const int words = (variables + 1 + 63) / 64;
    std::vector<std::vector<std::uint64_t>> rows(
        equations, std::vector<std::uint64_t>(words));
    for (int row = 0; row < equations; ++row) {
        assert(right[row] == 0 || right[row] == 1);
        for (int column : nonzero_columns[row]) {
            assert(0 <= column && column < variables);
            rows[row][column >> 6] ^= std::uint64_t{1} << (column & 63);
        }
        if (right[row]) {
            rows[row][variables >> 6] |= std::uint64_t{1} << (variables & 63);
        }
    }

    std::vector<int> pivot_row(variables, -1);
    int rank = 0;
    for (int column = 0; column < variables && rank < equations; ++column) {
        int pivot = rank;
        while (pivot < equations &&
               ((rows[pivot][column >> 6] >> (column & 63)) & 1U) == 0) {
            ++pivot;
        }
        if (pivot == equations) continue;
        std::swap(rows[pivot], rows[rank]);
        for (int row = rank + 1; row < equations; ++row) {
            if (((rows[row][column >> 6] >> (column & 63)) & 1U) == 0) continue;
            for (int word = column >> 6; word < words; ++word) {
                rows[row][word] ^= rows[rank][word];
            }
        }
        pivot_row[column] = rank++;
    }
    for (int row = rank; row < equations; ++row) {
        bool has_coefficient = false;
        for (int word = 0; word < (variables + 63) / 64; ++word) {
            std::uint64_t value = rows[row][word];
            if (word == variables >> 6 && (variables & 63) != 0) {
                value &= (std::uint64_t{1} << (variables & 63)) - 1;
            }
            has_coefficient |= value != 0;
        }
        const bool value =
            (rows[row][variables >> 6] >> (variables & 63)) & 1U;
        if (!has_coefficient && value) return std::nullopt;
    }

    std::vector<int> solution(variables);
    for (int column = variables - 1; column >= 0; --column) {
        if (pivot_row[column] == -1) continue;
        const int row = pivot_row[column];
        int value =
            (rows[row][variables >> 6] >> (variables & 63)) & 1U;
        for (int next = column + 1; next < variables; ++next) {
            value ^= solution[next] &
                     static_cast<int>((rows[row][next >> 6] >> (next & 63)) & 1U);
        }
        solution[column] = value;
    }
    return solution;
}

}
