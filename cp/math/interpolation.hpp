#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n)。lagrange_consecutive(values, x): f(i)=values[i]の次数<n多項式のf(x)を返す。
/// Tは体であり、0,1,...,n-1が相異なること。xがこの範囲なら対応値を返す。
template <class T>
T lagrange_consecutive(const std::vector<T>& values, long long x) {
    const int n = static_cast<int>(values.size());
    if (0 <= x && x < n) return values[x];
    if (n == 0) return T{};
    std::vector<T> prefix(static_cast<std::size_t>(n) + 1, T{1});
    std::vector<T> suffix(static_cast<std::size_t>(n) + 1, T{1});
    for (int i = 0; i < n; ++i) prefix[i + 1] = prefix[i] * T{x - i};
    for (int i = n - 1; i >= 0; --i) suffix[i] = suffix[i + 1] * T{x - i};
    std::vector<T> inverse_factorial(static_cast<std::size_t>(n), T{1});
    T factorial{1};
    for (int i = 1; i < n; ++i) factorial *= T{i};
    inverse_factorial[n - 1] = T{1} / factorial;
    for (int i = n - 1; i > 0; --i) inverse_factorial[i - 1] = inverse_factorial[i] * T{i};
    T answer{};
    for (int i = 0; i < n; ++i) {
        T coefficient = prefix[i] * suffix[i + 1] * inverse_factorial[i] * inverse_factorial[n - 1 - i];
        if ((n - 1 - i) & 1) coefficient = -coefficient;
        answer += values[i] * coefficient;
    }
    return answer;
}

}
