#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 区分線形凸関数を傾き変化点で管理する: slopetrick<long long> f;
template<class T>
struct slopetrick {
    /// O(1)。f.min(): 関数の最小値を返す。
    T min() const { return minimum; }

    /// O(1)。f.add_constant(value): 関数全体へvalueを加える。
    void add_constant(T value) { minimum += value; }

    /// O(log n)。f.add_left(a): max(a-x, 0)を加える。
    void add_left(T a) {
        if (!right.empty() && top_right() < a) {
            const T previous = top_right();
            minimum += a - previous;
            right.pop();
            push_right(a);
            push_left(previous);
        } else {
            push_left(a);
        }
    }

    /// O(log n)。f.add_right(a): max(x-a, 0)を加える。
    void add_right(T a) {
        if (!left.empty() && a < top_left()) {
            const T previous = top_left();
            minimum += previous - a;
            left.pop();
            push_left(a);
            push_right(previous);
        } else {
            push_right(a);
        }
    }

    /// O(log n)。f.add_abs(a): |x-a|を加える。
    void add_abs(T a) {
        add_left(a);
        add_right(a);
    }

    /// O(1)。f.shift(a, b): g(x)=min_{x-b<=y<=x-a} f(y)へ更新する。a<=bであること。
    void shift(T a, T b) {
        assert(a <= b);
        left_offset += a;
        right_offset += b;
    }

    /// O(1)。f.argmin(): 最小点区間の左右端を返す。無限端はnullopt。
    std::pair<std::optional<T>, std::optional<T>> argmin() const {
        return {
            left.empty() ? std::nullopt : std::optional<T>{top_left()},
            right.empty() ? std::nullopt : std::optional<T>{top_right()}
        };
    }

    /// O(n log n)。f[x]: 点xでの関数値を返す。
    T operator[](T x) const {
        T answer = minimum;
        auto left_copy = left;
        auto right_copy = right;
        while (!left_copy.empty()) {
            answer += std::max<T>(0, left_copy.top() + left_offset - x);
            left_copy.pop();
        }
        while (!right_copy.empty()) {
            answer += std::max<T>(0, x - (right_copy.top() + right_offset));
            right_copy.pop();
        }
        return answer;
    }

private:
    T top_left() const { return left.top() + left_offset; }
    T top_right() const { return right.top() + right_offset; }
    void push_left(T value) { left.push(value - left_offset); }
    void push_right(T value) { right.push(value - right_offset); }

    T minimum{};
    T left_offset{};
    T right_offset{};
    std::priority_queue<T> left;
    std::priority_queue<T, std::vector<T>, std::greater<T>> right;
};

}
