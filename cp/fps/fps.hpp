#pragma once
#include <bits/stdc++.h>
#include "math/convolution.hpp"

namespace poe {

/// fps<mod> f(n): mod上の形式的冪級数をvectorと同じ添字で持つ。
/// f.inv(n), f.log(n), f.exp(n), f.pow(k,n)は先頭n項を返す。
template<int mod>
struct fps : std::vector<staticmodint<mod>> {
    using mint = staticmodint<mod>;
    using base = std::vector<mint>;
    using base::base;

    fps() = default;
    fps(const base& values) : base(values) {}
    fps(base&& values) : base(std::move(values)) {}

    /// O(1)。f.size_int(): 項数をintで返す。
    int size_int() const noexcept { return static_cast<int>(this->size()); }

    /// O(1)。f.coeff(i): 範囲外を0としてx^iの係数を返す。
    mint coeff(int index) const {
        return 0 <= index && index < size_int() ? (*this)[index] : mint{};
    }

    /// O(n)。f.prefix(n): 0埋めまたは切り詰めで先頭n項を返す。
    fps prefix(int size) const {
        assert(size >= 0);
        fps result(static_cast<std::size_t>(size));
        std::copy_n(this->begin(), std::min(size, size_int()), result.begin());
        return result;
    }

    /// O(n)。f.shrink(): 末尾の0を削除する。
    fps& shrink() {
        while (!this->empty() && this->back() == mint{}) this->pop_back();
        return *this;
    }

    /// O(n)。f += g: 係数ごとに加算し、長い方の項数に合わせる。
    fps& operator+=(const fps& other) {
        if (this->size() < other.size()) this->resize(other.size());
        for (int i = 0; i < other.size_int(); ++i) (*this)[i] += other[i];
        return *this;
    }

    /// O(n)。f -= g: 係数ごとに減算し、長い方の項数に合わせる。
    fps& operator-=(const fps& other) {
        if (this->size() < other.size()) this->resize(other.size());
        for (int i = 0; i < other.size_int(); ++i) (*this)[i] -= other[i];
        return *this;
    }

    /// O(M(n))。f *= g: 畳み込みで多項式積を代入する。
    fps& operator*=(const fps& other) {
        base left(this->begin(), this->end());
        base right(other.begin(), other.end());
        base::operator=(convolution(std::move(left), std::move(right)));
        return *this;
    }

    /// O(n)。f *= c: 全係数をc倍する。
    fps& operator*=(mint scalar) {
        for (mint& value : *this) value *= scalar;
        return *this;
    }

    /// O(n log mod)。f /= c: 全係数をcで割る。c!=0。
    fps& operator/=(mint scalar) { return *this *= scalar.inv(); }

    friend fps operator+(fps left, const fps& right) { return left += right; }
    friend fps operator-(fps left, const fps& right) { return left -= right; }
    friend fps operator*(fps left, const fps& right) { return left *= right; }
    friend fps operator*(fps value, mint scalar) { return value *= scalar; }
    friend fps operator*(mint scalar, fps value) { return value *= scalar; }
    friend fps operator/(fps value, mint scalar) { return value /= scalar; }

    /// O(n)。-f: 全係数の符号を反転する。
    fps operator-() const {
        fps result = *this;
        for (mint& value : result) value = -value;
        return result;
    }

    /// O(n)。f.diff(): 形式微分を返す。
    fps diff() const {
        if (this->empty()) return {};
        fps result(this->size() - 1);
        for (int i = 1; i < size_int(); ++i) result[i - 1] = (*this)[i] * i;
        return result;
    }

    /// O(n log mod)。f.integral(): 定数項0の形式積分を返す。n<mod。
    fps integral() const {
        assert(size_int() < mod);
        fps result(this->size() + 1);
        for (int i = 0; i < size_int(); ++i) result[i + 1] = (*this)[i] / (i + 1);
        return result;
    }

    /// O(M(n) log n)。f.inv(n): 1/f mod x^nを返す。f[0]!=0。
    fps inv(int size = -1) const {
        if (size < 0) size = size_int();
        assert(size >= 0);
        if (size == 0) return {};
        assert(!this->empty() && (*this)[0] != mint{});
        fps result{(*this)[0].inv()};
        for (int length = 1; length < size; length <<= 1) {
            const int next = std::min(size, length << 1);
            fps correction = this->prefix(next) * result;
            correction.resize(next);
            for (mint& value : correction) value = -value;
            correction[0] += 2;
            result = (result * correction).prefix(next);
        }
        return result.prefix(size);
    }

    /// O(M(n) log n)。f.div(g,n): f/g mod x^nを返す。g[0]!=0。
    fps div(const fps& denominator, int size = -1) const {
        if (size < 0) size = size_int();
        return ((*this) * denominator.inv(size)).prefix(size);
    }

    /// O(M(n) log n)。f.log(n): log(f) mod x^nを返す。f[0]=1、n<mod。
    fps log(int size = -1) const {
        if (size < 0) size = size_int();
        assert(size >= 0 && size < mod);
        if (size == 0) return {};
        assert(!this->empty() && (*this)[0] == mint{1});
        return (this->diff() * this->inv(size)).prefix(size - 1).integral();
    }

    /// O(M(n) log n)。f.exp(n): exp(f) mod x^nを返す。f[0]=0、n<mod。
    fps exp(int size = -1) const {
        if (size < 0) size = size_int();
        assert(size >= 0 && size < mod);
        if (size == 0) return {};
        assert(this->empty() || (*this)[0] == mint{});
        fps result{mint{1}};
        for (int length = 1; length < size; length <<= 1) {
            const int next = std::min(size, length << 1);
            fps correction = this->prefix(next) - result.log(next);
            correction[0] += 1;
            result = (result * correction).prefix(next);
        }
        return result.prefix(size);
    }

    /// O(M(n) log n)。f.pow(k,n): f^k mod x^nを返す。k>=0、n<mod。
    fps pow(long long exponent, int size = -1) const {
        if (size < 0) size = size_int();
        assert(exponent >= 0 && size >= 0 && size < mod);
        fps result(static_cast<std::size_t>(size));
        if (size == 0) return result;
        if (exponent == 0) {
            result[0] = 1;
            return result;
        }
        int order = 0;
        while (order < size_int() && (*this)[order] == mint{}) ++order;
        if (order == size_int() || order > (size - 1) / exponent) return result;
        const int shift = static_cast<int>(order * exponent);
        const mint lead = (*this)[order];
        fps normalized(static_cast<std::size_t>(size - shift));
        for (int i = 0; i < size_int() - order && i < size - shift; ++i) {
            normalized[i] = (*this)[order + i] / lead;
        }
        normalized = (normalized.log(size - shift) * mint{exponent}).exp(size - shift);
        normalized *= lead.pow(exponent);
        for (int i = 0; i < normalized.size_int(); ++i) result[shift + i] = normalized[i];
        return result;
    }

    /// O(n)。f.eval(x): 多項式としてxへ代入した値を返す。
    mint eval(mint x) const {
        mint result{};
        for (int i = size_int() - 1; i >= 0; --i) result = result * x + (*this)[i];
        return result;
    }
};

using fps998244353 = fps<998244353>;

}
