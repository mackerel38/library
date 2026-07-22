#pragma once
#include <bits/stdc++.h>
#include "math/convolution.hpp"
#include "math/modint.hpp"

namespace poe {

/// 構築O(n)、取得O(1)。q整数・q階乗・Gaussian二項係数を前計算する。
template <class T>
struct qcombination {
    /// O(n)時間・O(n)領域。0<=k<=nのq階乗と逆q階乗を前計算する。
    qcombination(int size, T q) {
        assert(size >= 0);
        q_ = q;
        factorial_.assign(static_cast<std::size_t>(size) + 1, T{1});
        inverse_factorial_.assign(static_cast<std::size_t>(size) + 1, T{1});
        std::vector<T> integers(static_cast<std::size_t>(size) + 1);
        T integer = 0;
        for (int value = 1; value <= size; ++value) {
            integer = integer * q_ + T{1};
            integers[value] = integer;
            factorial_[value] = factorial_[value - 1] * integer;
        }
        inverse_factorial_[size] = T{1} / factorial_[size];
        for (int value = size; value >= 1; --value) {
            inverse_factorial_[value - 1] =
                inverse_factorial_[value] * integers[value];
        }
    }

    /// O(1)。q整数[n]_q=1+q+...+q^(n-1)を返す。1<=n<=前計算上限。
    T integer(int n) const {
        assert(1 <= n && n < static_cast<int>(factorial_.size()));
        return factorial_[n] * inverse_factorial_[n - 1];
    }

    /// O(1)。q階乗[n]_q!を返す。0<=n<=前計算上限。
    T factorial(int n) const {
        assert(0 <= n && n < static_cast<int>(factorial_.size()));
        return factorial_[n];
    }

    /// O(1)。q階乗の逆数1/[n]_q!を返す。0<=n<=前計算上限。
    T inverse_factorial(int n) const {
        assert(0 <= n && n < static_cast<int>(factorial_.size()));
        return inverse_factorial_[n];
    }

    /// O(1)。Gaussian二項係数[n choose k]_qを返す。範囲外では0。
    T choose(int n, int k) const {
        if (n < 0 || k < 0 || k > n || n >= static_cast<int>(factorial_.size())) {
            return T{};
        }
        return factorial_[n] * inverse_factorial_[k] * inverse_factorial_[n - k];
    }

private:
    T q_;
    std::vector<T> factorial_;
    std::vector<T> inverse_factorial_;
};

/// O(D+R+M(R))。F_q上のs行D列行列で行空間が固定非零vectorを含まない個数を全0<=s<=Rで返す。
template <int mod>
std::vector<staticmodint<mod>> matrix_rowspace_avoiding_vector_counts(
    int maximum_rows,
    int dimension,
    int field_size
) {
    assert(maximum_rows >= 0 && dimension >= 1 && field_size >= 2);
    using mint = staticmodint<mod>;
    const int limit = std::max(maximum_rows, dimension);
    const qcombination<mint> combination(limit, mint{field_size});
    const int maximum_rank = std::min(maximum_rows, dimension - 1);

    std::vector<mint> left(static_cast<std::size_t>(maximum_rows) + 1);
    std::vector<mint> right(static_cast<std::size_t>(maximum_rows) + 1);
    mint rank_power = 1;
    mint next_power = field_size;
    mint field_minus_one_power = 1;
    for (int rank = 0; rank <= maximum_rank; ++rank) {
        left[rank] = rank_power * field_minus_one_power *
                     combination.inverse_factorial(dimension - 1 - rank) *
                     combination.inverse_factorial(rank);
        rank_power *= next_power;
        next_power *= field_size;
        field_minus_one_power *= field_size - 1;
    }
    for (int remaining = 0; remaining <= maximum_rows; ++remaining) {
        right[remaining] = combination.inverse_factorial(remaining);
    }
    const auto product = convolution(std::move(left), std::move(right));
    std::vector<mint> result(static_cast<std::size_t>(maximum_rows) + 1);
    const mint dimension_factorial = combination.factorial(dimension - 1);
    for (int rows = 0; rows <= maximum_rows; ++rows) {
        result[rows] = dimension_factorial * combination.factorial(rows) * product[rows];
    }
    return result;
}

}
