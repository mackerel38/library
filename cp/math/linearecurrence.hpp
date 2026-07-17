#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(k^2 log n)。任意半環上のk項漸化式a[i]=add_j multiply(c[j-1],a[i-j])のa[n]。
template <class T, class Add, class Multiply>
T semiring_recurrence(
    const std::vector<T>& initial,
    const std::vector<T>& coefficient,
    long long index,
    const T& zero,
    const T& one,
    Add add,
    Multiply multiply
) {
    const int order = static_cast<int>(coefficient.size());
    assert(order > 0 && static_cast<int>(initial.size()) == order && index >= 0);
    if (index < order) return initial[index];
    auto polynomial_multiply =
        [&](const std::vector<T>& left, const std::vector<T>& right) {
            std::vector<T> product(static_cast<std::size_t>(2 * order - 1), zero);
            for (int i = 0; i < order; ++i) {
                for (int j = 0; j < order; ++j) {
                    product[i + j] = add(
                        product[i + j], multiply(left[i], right[j]));
                }
            }
            for (int degree = 2 * order - 2; degree >= order; --degree) {
                for (int j = 1; j <= order; ++j) {
                    product[degree - j] = add(
                        product[degree - j],
                        multiply(product[degree], coefficient[j - 1]));
                }
            }
            product.resize(order);
            return product;
        };
    std::vector<T> result(static_cast<std::size_t>(order), zero);
    std::vector<T> base(static_cast<std::size_t>(order), zero);
    result[0] = one;
    if (order == 1) base[0] = coefficient[0];
    else base[1] = one;
    while (index > 0) {
        if (index & 1) result = polynomial_multiply(result, base);
        index >>= 1;
        if (index > 0) base = polynomial_multiply(base, base);
    }
    T answer = zero;
    for (int i = 0; i < order; ++i) {
        answer = add(answer, multiply(result[i], initial[i]));
    }
    return answer;
}

/// O(k^2 log n)。linear_recurrence(initial, coefficient, n)でa[n]を返す。
/// a[i]=sum_{j=1..k} coefficient[j-1]*a[i-j]、initial.size()==k。
template <class T>
T linear_recurrence(
    const std::vector<T>& initial,
    const std::vector<T>& coefficient,
    long long index
) {
    return semiring_recurrence(
        initial, coefficient, index, T{}, T{1},
        std::plus<T>{}, std::multiplies<T>{});
}

/// O(k^2 log n)。a[i]=max_j(a[i-j]+coefficient[j-1])で定まるmax-plus漸化式のa[n]。
template <class T>
T maxplus_recurrence(
    const std::vector<T>& initial,
    const std::vector<T>& coefficient,
    long long index
) {
    const T negative_infinity = std::numeric_limits<T>::lowest();
    return semiring_recurrence(
        initial, coefficient, index, negative_infinity, T{},
        [](const T& left, const T& right) { return std::max(left, right); },
        [negative_infinity](const T& left, const T& right) {
            if (left == negative_infinity || right == negative_infinity) {
                return negative_infinity;
            }
            return left + right;
        });
}

}
