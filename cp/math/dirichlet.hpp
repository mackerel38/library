#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n log n)。添字1始まりの列left,rightのDirichlet畳み込みをn未満で返す。
template<class T>
std::vector<T> dirichlet_convolution(const std::vector<T>& left, const std::vector<T>& right) {
    assert(left.size() == right.size());
    std::vector<T> result(left.size());
    const int limit = static_cast<int>(left.size()) - 1;
    for (int i = 1; i <= limit; ++i) {
        for (int j = 1; j <= limit / i; ++j) {
            result[i * j] += left[i] * right[j];
        }
    }
    return result;
}

/// O(n log n)。values[1]が可逆な列のDirichlet畳み込み逆元を返す。添字0は使わない。
template<class T>
std::vector<T> dirichlet_inverse(const std::vector<T>& values) {
    assert(values.size() >= 2);
    std::vector<T> result(values.size());
    const int limit = static_cast<int>(values.size()) - 1;
    const T inverse_constant = T{1} / values[1];
    result[1] = inverse_constant;
    for (int i = 1; i <= limit; ++i) {
        for (int j = 2; j <= limit / i; ++j) {
            result[i * j] -= result[i] * values[j] * inverse_constant;
        }
    }
    return result;
}

/// O(n)。omega[n]=nの素因数の重複込み個数となる表を返す。
inline std::vector<int> prime_omega_table(int limit) {
    assert(limit >= 0);
    std::vector<int> least(limit + 1), omega(limit + 1), primes;
    for (int value = 2; value <= limit; ++value) {
        if (least[value] == 0) {
            least[value] = value;
            primes.push_back(value);
        }
        omega[value] = omega[value / least[value]] + 1;
        for (const int prime : primes) {
            if (prime > least[value] || prime > limit / value) break;
            least[value * prime] = prime;
        }
    }
    return omega;
}

/// O(n log n)。values[1]=1の形式Dirichlet級数についてlog(values)を返す。
template<class T>
std::vector<T> dirichlet_log(const std::vector<T>& values) {
    assert(values.size() >= 2);
    assert(values[1] == T{1});
    const int limit = static_cast<int>(values.size()) - 1;
    const auto omega = prime_omega_table(limit);
    std::vector<T> derivative(values.size());
    for (int value = 2; value <= limit; ++value) {
        derivative[value] = values[value] * T{omega[value]};
    }
    auto result = dirichlet_convolution(derivative, dirichlet_inverse(values));
    for (int value = 2; value <= limit; ++value) {
        result[value] /= T{omega[value]};
    }
    return result;
}

/// O(u log u)。frequency[w]種類の重みwを使う有向necklaceを、積ごとに数える。frequency[1]=0。
template<class T>
std::vector<T> product_necklace_counts(const std::vector<T>& frequency) {
    assert(frequency.size() >= 2);
    assert(frequency[1] == T{});
    const int limit = static_cast<int>(frequency.size()) - 1;

    std::vector<T> one_minus_frequency(frequency.size());
    one_minus_frequency[1] = T{1};
    for (int value = 2; value <= limit; ++value) {
        one_minus_frequency[value] = -frequency[value];
    }
    auto logarithm = dirichlet_log(one_minus_frequency);
    for (int value = 2; value <= limit; ++value) logarithm[value] = -logarithm[value];

    int max_length = 0;
    for (long long power = 1; power <= limit; power *= 2) ++max_length;
    std::vector<int> phi(max_length + 1);
    for (int i = 0; i <= max_length; ++i) phi[i] = i;
    for (int prime = 2; prime <= max_length; ++prime) {
        if (phi[prime] != prime) continue;
        for (int multiple = prime; multiple <= max_length; multiple += prime) {
            phi[multiple] -= phi[multiple] / prime;
        }
    }

    std::vector<T> result(frequency.size());
    for (int base = 2; base <= limit; ++base) {
        long long product = base;
        for (int length = 1; product <= limit; ++length) {
            result[product] += logarithm[base] * T{phi[length]} / T{length};
            if (product > limit / base) break;
            product *= base;
        }
    }
    return result;
}

}
