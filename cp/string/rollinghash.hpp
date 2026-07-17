#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 2^61-1 modの列hash: rollinghash<char> hash(text); 構築O(n)、部分列hash O(1)、LCP O(log n)。
template <class T = char>
struct rollinghash {
    using value_type = unsigned long long;

    /// O(n)。valuesのprefix hashとbase冪を構築する。
    explicit rollinghash(const std::vector<T>& values) : values_(values) {
        prefix_.resize(size() + 1);
        power_.resize(size() + 1, 1);
        for (int index = 0; index < size(); ++index) {
            power_[index + 1] = multiply(power_[index], base);
            prefix_[index + 1] =
                add(multiply(prefix_[index], base), encode(values_[index]));
        }
    }

    /// O(n)。文字列textから構築する。
    explicit rollinghash(const std::basic_string<T>& text)
        : rollinghash(std::vector<T>(text.begin(), text.end())) {}

    /// O(1)。列長を返す。
    int size() const noexcept { return static_cast<int>(values_.size()); }

    /// O(1)。hash[index]で元の列の要素を返す。
    const T& operator[](int index) const {
        assert(0 <= index && index < size());
        return values_[index];
    }

    /// O(1)。半開区間[left,right)のhashを返す。
    value_type hash(int left, int right) const {
        assert(0 <= left && left <= right && right <= size());
        return subtract(prefix_[right], multiply(prefix_[left], power_[right - left]));
    }

    /// O(1)。left_hashの後ろへ長さright_lengthのright_hashを連結したhashを返す。
    value_type concat(value_type left_hash, value_type right_hash,
                      int right_length) const {
        assert(0 <= right_length && right_length <= size());
        return add(multiply(left_hash, power_[right_length]), right_hash);
    }

    /// O(1)。二つの半開部分列がhash上で等しいか返す。
    bool equal(int left, int right, const rollinghash& other, int other_left,
               int other_right) const {
        assert(right - left == other_right - other_left);
        return hash(left, right) == other.hash(other_left, other_right);
    }

    /// O(log length)。二つの半開部分列のLCP長を返す。
    int lcp(int left, int right, const rollinghash& other, int other_left,
            int other_right) const {
        assert(0 <= left && left <= right && right <= size());
        assert(0 <= other_left && other_left <= other_right && other_right <= other.size());
        int low = 0;
        int high = std::min(right - left, other_right - other_left) + 1;
        while (high - low > 1) {
            const int middle = (low + high) / 2;
            if (hash(left, left + middle) ==
                other.hash(other_left, other_left + middle)) {
                low = middle;
            } else {
                high = middle;
            }
        }
        return low;
    }

private:
    static constexpr value_type modulus = (1ULL << 61) - 1;
    static constexpr value_type base = 1'000'003;

    static value_type encode(const T& value) {
        return static_cast<value_type>(std::hash<T>{}(value)) % (modulus - 1) + 1;
    }

    static value_type add(value_type left, value_type right) {
        const value_type value = left + right;
        return value >= modulus ? value - modulus : value;
    }

    static value_type subtract(value_type left, value_type right) {
        return left >= right ? left - right : left + modulus - right;
    }

    static value_type multiply(value_type left, value_type right) {
        return static_cast<value_type>(
            static_cast<unsigned __int128>(left) * right % modulus);
    }

    std::vector<T> values_;
    std::vector<value_type> prefix_;
    std::vector<value_type> power_;
};

}
