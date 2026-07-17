#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 合成数法でも使えるPascal二項係数表: pascalcombination comb(n, modulus)。
struct pascalcombination {
    /// O(n^2)時間・O(n^2)空間。0<=a<=nのaCbを任意の正の法で前計算する。
    pascalcombination(int size, int modulus)
        : size_(size), modulus_(modulus),
          values_(static_cast<std::size_t>(size + 1) * (size + 2) / 2) {
        assert(size >= 0 && modulus > 0);
        for (int n = 0; n <= size_; ++n) {
            value(n, 0) = 1 % modulus_;
            value(n, n) = 1 % modulus_;
            for (int k = 1; k < n; ++k) {
                value(n, k) = (static_cast<long long>(choose(n - 1, k - 1))
                               + choose(n - 1, k)) % modulus_;
            }
        }
    }

    /// O(1)。nCkを返す。範囲外では0。
    int choose(int n, int k) const {
        if (n < 0 || n > size_ || k < 0 || k > n) return 0;
        return values_[offset(n) + k];
    }

    /// O(m)。countsを出現数とする多項係数(sum counts)!/prod(count!)を返す。
    template <std::ranges::input_range Range>
    int multinomial(const Range& counts) const {
        long long result = 1 % modulus_;
        int sum = 0;
        for (const auto count : counts) {
            assert(count >= 0 && sum + count <= size_);
            sum += count;
            result = result * choose(sum, count) % modulus_;
        }
        return static_cast<int>(result);
    }

    /// O(1)。前計算上限を返す。
    int size() const noexcept { return size_; }

    /// O(1)。法を返す。
    int mod() const noexcept { return modulus_; }

private:
    static std::size_t offset(int n) {
        return static_cast<std::size_t>(n) * (n + 1) / 2;
    }

    int& value(int n, int k) {
        return values_[offset(n) + k];
    }

    int size_;
    int modulus_;
    std::vector<int> values_;
};

/// modint等で二項係数を前計算する: combination<mint> comb(n); 構築O(n)、取得O(1)。
template <class T>
struct combination {
    /// O(n)。0<=k<=nの階乗と逆階乗を前計算する。
    explicit combination(int size) {
        assert(size >= 0);
        factorial_.resize(static_cast<std::size_t>(size) + 1);
        inverse_factorial_.resize(static_cast<std::size_t>(size) + 1);
        factorial_[0] = T{1};
        for (int i = 1; i <= size; ++i) factorial_[i] = factorial_[i - 1] * T{i};
        inverse_factorial_[size] = T{1} / factorial_[size];
        for (int i = size; i > 0; --i) inverse_factorial_[i - 1] = inverse_factorial_[i] * T{i};
    }

    /// O(1)。n!を返す。0<=n<=前計算上限。
    T factorial(int n) const { return factorial_[n]; }

    /// O(1)。nPkを返す。範囲外のkでは0を返す。
    T permutation(int n, int k) const {
        if (n < 0 || k < 0 || k > n || n >= static_cast<int>(factorial_.size())) return T{};
        return factorial_[n] * inverse_factorial_[n - k];
    }

    /// O(1)。nCkを返す。範囲外のkでは0を返す。
    T choose(int n, int k) const {
        if (n < 0 || k < 0 || k > n || n >= static_cast<int>(factorial_.size())) return T{};
        return factorial_[n] * inverse_factorial_[k] * inverse_factorial_[n - k];
    }

    /// O(k)。巨大なnに対するnCkを返す。0<=k<=前計算上限かつk!が可逆であること。
    template <std::integral Integer>
    T choose_large(Integer n, int k) const {
        if (n < 0 || k < 0 || static_cast<Integer>(k) > n ||
            k >= static_cast<int>(inverse_factorial_.size())) return T{};
        T result = inverse_factorial_[k];
        for (int i = 0; i < k; ++i) result *= T{n - static_cast<Integer>(i)};
        return result;
    }

    /// O(1)。重複組合せnHkを返す。
    T multichoose(int n, int k) const {
        if (n < 0 || k < 0) return T{};
        if (n == 0) return k == 0 ? T{1} : T{};
        return choose(n + k - 1, k);
    }

private:
    std::vector<T> factorial_;
    std::vector<T> inverse_factorial_;
};

}
