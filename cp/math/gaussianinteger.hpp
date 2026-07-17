#pragma once
#include <bits/stdc++.h>

namespace poe {

/// ガウス整数・複素数型: gaussianinteger<T>{real, imaginary}; 四則の加減乗算O(1)。
template <class T>
struct gaussianinteger {
    T real{};
    T imaginary{};

    auto operator<=>(const gaussianinteger&) const = default;

    /// O(1)。複素共役を返す。
    gaussianinteger conjugate() const { return {real, -imaginary}; }

    /// O(1)。real^2+imaginary^2を返す。
    T norm() const { return real * real + imaginary * imaginary; }

    gaussianinteger& operator+=(const gaussianinteger& other) {
        real += other.real;
        imaginary += other.imaginary;
        return *this;
    }

    gaussianinteger& operator-=(const gaussianinteger& other) {
        real -= other.real;
        imaginary -= other.imaginary;
        return *this;
    }

    gaussianinteger& operator*=(const gaussianinteger& other) {
        const T next_real = real * other.real - imaginary * other.imaginary;
        const T next_imaginary = real * other.imaginary + imaginary * other.real;
        real = next_real;
        imaginary = next_imaginary;
        return *this;
    }

    friend gaussianinteger operator+(gaussianinteger left, const gaussianinteger& right) {
        return left += right;
    }

    friend gaussianinteger operator-(gaussianinteger left, const gaussianinteger& right) {
        return left -= right;
    }

    friend gaussianinteger operator*(gaussianinteger left, const gaussianinteger& right) {
        return left *= right;
    }
};

/// O(log exponent)。ガウス整数baseの非負整数乗を返す。
template <class T>
gaussianinteger<T> gaussian_power(
    gaussianinteger<T> base,
    unsigned long long exponent
) {
    gaussianinteger<T> result{T{1}, T{}};
    while (exponent > 0) {
        if (exponent & 1) result *= base;
        exponent >>= 1;
        if (exponent > 0) base *= base;
    }
    return result;
}

/// O(1)。各成分を[0,modulus)へ正規化する。
template <std::integral T>
gaussianinteger<T> gaussian_mod(gaussianinteger<T> value, T modulus) {
    assert(modulus > 0);
    value.real %= modulus;
    value.imaginary %= modulus;
    if (value.real < 0) value.real += modulus;
    if (value.imaginary < 0) value.imaginary += modulus;
    return value;
}

}
