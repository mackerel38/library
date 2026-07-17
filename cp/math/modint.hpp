#pragma once
#include <bits/stdc++.h>
#include "math/math.hpp"

namespace poe {

/// 固定modの整数: staticmodint<998244353> x; 四則演算O(1)、除算O(log mod)。
template <int modulus>
struct staticmodint {
    static_assert(1 <= modulus);

    /// O(1)。0で初期化する。
    staticmodint() = default;

    /// O(1)。valueをmodulusで正規化して代入する。
    template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
    staticmodint(T value) {
        if constexpr (std::is_signed_v<T>) {
            long long normalized = static_cast<long long>(value % modulus);
            if (normalized < 0) {
                normalized += modulus;
            }
            value_ = static_cast<unsigned int>(normalized);
        } else {
            value_ = static_cast<unsigned int>(value % static_cast<unsigned long long>(modulus));
        }
    }

    /// O(1)。剰余を取らずvalueを格納する。0<=value<mod()が必要。
    static staticmodint raw(unsigned int value) {
        staticmodint result;
        result.value_ = value;
        return result;
    }

    /// O(1)。法を返す。
    static constexpr int mod() {
        return modulus;
    }

    /// O(1)。[0, mod())の値を返す。
    unsigned int val() const {
        return value_;
    }

    /// O(1)。++xを行う。
    staticmodint& operator++() {
        if (++value_ == static_cast<unsigned int>(modulus)) {
            value_ = 0;
        }
        return *this;
    }

    /// O(1)。x++を行う。
    staticmodint operator++(int) {
        staticmodint old = *this;
        ++*this;
        return old;
    }

    /// O(1)。--xを行う。
    staticmodint& operator--() {
        if (value_ == 0) {
            value_ = modulus;
        }
        --value_;
        return *this;
    }

    /// O(1)。x--を行う。
    staticmodint operator--(int) {
        staticmodint old = *this;
        --*this;
        return old;
    }

    staticmodint& operator+=(const staticmodint& other) {
        value_ += other.value_;
        if (value_ >= static_cast<unsigned int>(modulus)) {
            value_ -= modulus;
        }
        return *this;
    }

    staticmodint& operator-=(const staticmodint& other) {
        value_ -= other.value_;
        if (value_ >= static_cast<unsigned int>(modulus)) {
            value_ += modulus;
        }
        return *this;
    }

    staticmodint& operator*=(const staticmodint& other) {
        value_ = static_cast<unsigned int>(
            static_cast<unsigned long long>(value_) * other.value_ % modulus);
        return *this;
    }

    staticmodint& operator/=(const staticmodint& other) {
        return *this *= other.inv();
    }

    staticmodint operator+() const {
        return *this;
    }

    staticmodint operator-() const {
        return value_ == 0 ? *this : raw(modulus - value_);
    }

    friend staticmodint operator+(staticmodint left, const staticmodint& right) {
        return left += right;
    }

    friend staticmodint operator-(staticmodint left, const staticmodint& right) {
        return left -= right;
    }

    friend staticmodint operator*(staticmodint left, const staticmodint& right) {
        return left *= right;
    }

    friend staticmodint operator/(staticmodint left, const staticmodint& right) {
        return left /= right;
    }

    friend bool operator==(const staticmodint& left, const staticmodint& right) {
        return left.value_ == right.value_;
    }

    friend bool operator!=(const staticmodint& left, const staticmodint& right) {
        return !(left == right);
    }

    /// O(log exponent)。x^exponentを返す。exponent>=0。
    staticmodint pow(long long exponent) const {
        assert(exponent >= 0);
        staticmodint value = *this;
        staticmodint result = 1;
        while (exponent > 0) {
            if (exponent & 1) {
                result *= value;
            }
            value *= value;
            exponent >>= 1;
        }
        return result;
    }

    /// O(log mod)。乗法逆元を返す。gcd(val(), mod())=1が必要。
    staticmodint inv() const {
        return inv_mod(value_, modulus);
    }

    friend std::istream& operator>>(std::istream& input, staticmodint& value) {
        long long raw_value;
        input >> raw_value;
        value = staticmodint(raw_value);
        return input;
    }

    friend std::ostream& operator<<(std::ostream& output, const staticmodint& value) {
        return output << value.val();
    }

private:
    unsigned int value_ = 0;
};

/// 実行時modの整数: dynamicmodint<id>::set_mod(mod); 四則演算O(1)、除算O(log mod)。
template <int id>
struct dynamicmodint {
    /// O(1)。0で初期化する。
    dynamicmodint() = default;

    /// O(1)。valueをmod()で正規化して代入する。
    template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
    dynamicmodint(T value) {
        const int modulus = mod();
        if constexpr (std::is_signed_v<T>) {
            long long normalized = static_cast<long long>(value % modulus);
            if (normalized < 0) {
                normalized += modulus;
            }
            value_ = static_cast<unsigned int>(normalized);
        } else {
            value_ = static_cast<unsigned int>(value % static_cast<unsigned long long>(modulus));
        }
    }

    /// O(1)。以後の法を設定する。modulus>=1。
    static void set_mod(int modulus) {
        assert(modulus >= 1);
        modulus_ = modulus;
    }

    /// O(1)。現在の法を返す。
    static int mod() {
        return modulus_;
    }

    /// O(1)。剰余を取らずvalueを格納する。0<=value<mod()が必要。
    static dynamicmodint raw(unsigned int value) {
        dynamicmodint result;
        result.value_ = value;
        return result;
    }

    /// O(1)。[0, mod())の値を返す。
    unsigned int val() const {
        return value_;
    }

    dynamicmodint& operator++() {
        if (++value_ == static_cast<unsigned int>(mod())) {
            value_ = 0;
        }
        return *this;
    }

    dynamicmodint operator++(int) {
        dynamicmodint old = *this;
        ++*this;
        return old;
    }

    dynamicmodint& operator--() {
        if (value_ == 0) {
            value_ = mod();
        }
        --value_;
        return *this;
    }

    dynamicmodint operator--(int) {
        dynamicmodint old = *this;
        --*this;
        return old;
    }

    dynamicmodint& operator+=(const dynamicmodint& other) {
        value_ += other.value_;
        if (value_ >= static_cast<unsigned int>(mod())) {
            value_ -= mod();
        }
        return *this;
    }

    dynamicmodint& operator-=(const dynamicmodint& other) {
        value_ -= other.value_;
        if (value_ >= static_cast<unsigned int>(mod())) {
            value_ += mod();
        }
        return *this;
    }

    dynamicmodint& operator*=(const dynamicmodint& other) {
        value_ = static_cast<unsigned int>(
            static_cast<unsigned long long>(value_) * other.value_ % mod());
        return *this;
    }

    dynamicmodint& operator/=(const dynamicmodint& other) {
        return *this *= other.inv();
    }

    dynamicmodint operator+() const {
        return *this;
    }

    dynamicmodint operator-() const {
        return value_ == 0 ? *this : raw(mod() - value_);
    }

    friend dynamicmodint operator+(dynamicmodint left, const dynamicmodint& right) {
        return left += right;
    }

    friend dynamicmodint operator-(dynamicmodint left, const dynamicmodint& right) {
        return left -= right;
    }

    friend dynamicmodint operator*(dynamicmodint left, const dynamicmodint& right) {
        return left *= right;
    }

    friend dynamicmodint operator/(dynamicmodint left, const dynamicmodint& right) {
        return left /= right;
    }

    friend bool operator==(const dynamicmodint& left, const dynamicmodint& right) {
        return left.value_ == right.value_;
    }

    friend bool operator!=(const dynamicmodint& left, const dynamicmodint& right) {
        return !(left == right);
    }

    /// O(log exponent)。x^exponentを返す。exponent>=0。
    dynamicmodint pow(long long exponent) const {
        assert(exponent >= 0);
        dynamicmodint value = *this;
        dynamicmodint result = 1;
        while (exponent > 0) {
            if (exponent & 1) {
                result *= value;
            }
            value *= value;
            exponent >>= 1;
        }
        return result;
    }

    /// O(log mod)。乗法逆元を返す。gcd(val(), mod())=1が必要。
    dynamicmodint inv() const {
        return inv_mod(value_, mod());
    }

    friend std::istream& operator>>(std::istream& input, dynamicmodint& value) {
        long long raw_value;
        input >> raw_value;
        value = dynamicmodint(raw_value);
        return input;
    }

    friend std::ostream& operator<<(std::ostream& output, const dynamicmodint& value) {
        return output << value.val();
    }

private:
    inline static int modulus_ = 998244353;
    unsigned int value_ = 0;
};

/// 法998244353の固定modint: modint998244353 value; 各演算量はstaticmodintと同じ。
using modint998244353 = staticmodint<998244353>;

/// 法1000000007の固定modint: modint1000000007 value; 各演算量はstaticmodintと同じ。
using modint1000000007 = staticmodint<1000000007>;

/// 既定idの実行時modint: modint::set_mod(modulus); 各演算量はdynamicmodintと同じ。
using modint = dynamicmodint<-1>;

}
