#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 加算・乗算を持つ型の行列: matrix<mint> a(h, w); 乗算O(hwk)。
template <class T>
struct matrix {
    /// O(hw)。h行w列をT{}で初期化する。
    matrix(int height, int width) : height_(height), width_(width) {
        assert(height >= 0 && width >= 0);
        data_.assign(height, std::vector<T>(width));
    }

    /// O(hw)。二次元vectorから構築する。
    explicit matrix(std::vector<std::vector<T>> values)
        : height_(static_cast<int>(values.size())),
          width_(values.empty() ? 0 : static_cast<int>(values[0].size())),
          data_(std::move(values)) {
        for (const auto& row : data_) assert(static_cast<int>(row.size()) == width_);
    }

    /// O(n^2)。n次単位行列を返す。
    static matrix identity(int size) {
        assert(size >= 0);
        matrix result(size, size);
        for (int i = 0; i < size; ++i) result[i][i] = T{1};
        return result;
    }

    /// O(1)。行数を返す。
    int height() const noexcept { return height_; }

    /// O(1)。列数を返す。
    int width() const noexcept { return width_; }

    /// O(1)。row行への参照を返す。
    std::vector<T>& operator[](int row) { return data_[row]; }

    /// O(1)。row行へのconst参照を返す。
    const std::vector<T>& operator[](int row) const { return data_[row]; }

    /// O(hwk)。行列積を返す。
    friend matrix operator*(const matrix& left, const matrix& right) {
        assert(left.width_ == right.height_);
        matrix result(left.height_, right.width_);
        for (int i = 0; i < left.height_; ++i) {
            for (int k = 0; k < left.width_; ++k) {
                for (int j = 0; j < right.width_; ++j) {
                    result[i][j] += left[i][k] * right[k][j];
                }
            }
        }
        return result;
    }

    /// O(n^3 log exponent)。正方行列の非負整数乗を返す。
    matrix pow(long long exponent) const {
        assert(height_ == width_ && exponent >= 0);
        matrix base = *this;
        matrix result = identity(height_);
        while (exponent > 0) {
            if (exponent & 1) result = result * base;
            exponent >>= 1;
            if (exponent > 0) base = base * base;
        }
        return result;
    }

private:
    int height_;
    int width_;
    std::vector<std::vector<T>> data_;
};

/// O(hw)。matrixと列vectorの積を返す。
template <class T>
std::vector<T> matrix_vector_product(
    const matrix<T>& coefficients,
    const std::vector<T>& values
) {
    assert(coefficients.width() == static_cast<int>(values.size()));
    std::vector<T> result(coefficients.height());
    for (int row = 0; row < coefficients.height(); ++row) {
        for (int column = 0; column < coefficients.width(); ++column) {
            result[row] += coefficients[row][column] * values[column];
        }
    }
    return result;
}

/// O(d^3 log target+q d^2 log target)。通常行列で進み、指定時刻だけevent(state,time)を適用する。
template <class T, class Event>
requires std::invocable<Event&, std::vector<T>&, unsigned long long>
std::vector<T> sparse_matrix_events(
    std::vector<T> initial,
    const matrix<T>& transition,
    unsigned long long target,
    const std::vector<unsigned long long>& event_positions,
    Event event
) {
    const int dimension = static_cast<int>(initial.size());
    assert(transition.height() == dimension && transition.width() == dimension);
    unsigned long long previous = 0;
    for (const auto position : event_positions) {
        assert(previous < position && position <= target);
        previous = position;
    }

    std::vector<matrix<T>> powers;
    if (target > 0) {
        powers.push_back(transition);
        for (unsigned long long length = 1; length <= target / 2; length <<= 1) {
            powers.push_back(powers.back() * powers.back());
            if (length > std::numeric_limits<unsigned long long>::max() / 2) break;
        }
    }
    const auto advance = [&](std::vector<T>& state, unsigned long long steps) {
        for (int bit = 0; steps > 0; ++bit, steps >>= 1) {
            if (steps & 1) state = matrix_vector_product(powers[bit], state);
        }
    };

    std::vector<T> state = std::move(initial);
    previous = 0;
    for (const auto position : event_positions) {
        advance(state, position - previous - 1);
        std::invoke(event, state, position);
        previous = position;
    }
    advance(state, target - previous);
    return state;
}

/// O(d^3 log target+q d^2 log target)。指定時刻だけexception_transitionへ置き換えてtargetまで進む。
template <class T>
std::vector<T> sparse_matrix_transitions(
    std::vector<T> initial,
    const matrix<T>& transition,
    const matrix<T>& exception_transition,
    unsigned long long target,
    const std::vector<unsigned long long>& exception_positions
) {
    assert(exception_transition.height() == transition.height()
           && exception_transition.width() == transition.width());
    return sparse_matrix_events(
        std::move(initial), transition, target, exception_positions,
        [&](std::vector<T>& state, unsigned long long) {
            state = matrix_vector_product(exception_transition, state);
        }
    );
}

/// O(hwk)。zero・add・multiplyで定めた半環上の行列積を返す。
template <class T, class Add, class Multiply>
matrix<T> semiring_matrix_product(const matrix<T>& left,
                                  const matrix<T>& right,
                                  const T& zero,
                                  Add add,
                                  Multiply multiply) {
    assert(left.width() == right.height());
    matrix<T> result(left.height(), right.width());
    for (int i = 0; i < result.height(); ++i)
        std::fill(result[i].begin(), result[i].end(), zero);
    for (int i = 0; i < left.height(); ++i) {
        for (int k = 0; k < left.width(); ++k) {
            for (int j = 0; j < right.width(); ++j) {
                result[i][j] = add(result[i][j], multiply(left[i][k], right[k][j]));
            }
        }
    }
    return result;
}

/// O(n^3 log exponent)。zero・one・演算を指定した半環上の正方行列累乗を返す。
template <class T, class Add, class Multiply>
matrix<T> semiring_matrix_power(matrix<T> base,
                                unsigned long long exponent,
                                const T& zero,
                                const T& one,
                                Add add,
                                Multiply multiply) {
    assert(base.height() == base.width());
    matrix<T> result(base.height(), base.width());
    for (int i = 0; i < result.height(); ++i) {
        std::fill(result[i].begin(), result[i].end(), zero);
        result[i][i] = one;
    }
    while (exponent > 0) {
        if (exponent & 1)
            result = semiring_matrix_product(result, base, zero, add, multiply);
        exponent >>= 1;
        if (exponent > 0)
            base = semiring_matrix_product(base, base, zero, add, multiply);
    }
    return result;
}

/// O(hwk)。min-plus半環で行列積を返す。infinity以上は到達不能として扱う。
template <class T>
matrix<T> minplus_matrix_product(
    const matrix<T>& left,
    const matrix<T>& right,
    T infinity = std::numeric_limits<T>::max() / 4
) {
    const auto add = [](const T& first, const T& second) {
        return std::min(first, second);
    };
    const auto multiply = [infinity](const T& first, const T& second) {
        if (first >= infinity || second >= infinity) return infinity;
        if constexpr (std::is_integral_v<T> && sizeof(T) <= 8) {
            const __int128 value = static_cast<__int128>(first) + second;
            return static_cast<T>(std::clamp(
                value,
                static_cast<__int128>(std::numeric_limits<T>::lowest()),
                static_cast<__int128>(infinity)
            ));
        } else {
            return std::min<T>(infinity, first + second);
        }
    };
    return semiring_matrix_product(left, right, infinity, add, multiply);
}

/// O(n^3 log exponent)。ちょうどexponent辺の最小費用を表すmin-plus行列累乗を返す。
template <class T>
matrix<T> minplus_matrix_power(
    matrix<T> base,
    unsigned long long exponent,
    T infinity = std::numeric_limits<T>::max() / 4
) {
    const auto add = [](const T& first, const T& second) {
        return std::min(first, second);
    };
    const auto multiply = [infinity](const T& first, const T& second) {
        if (first >= infinity || second >= infinity) return infinity;
        if constexpr (std::is_integral_v<T> && sizeof(T) <= 8) {
            const __int128 value = static_cast<__int128>(first) + second;
            return static_cast<T>(std::clamp(
                value,
                static_cast<__int128>(std::numeric_limits<T>::lowest()),
                static_cast<__int128>(infinity)
            ));
        } else {
            return std::min<T>(infinity, first + second);
        }
    };
    return semiring_matrix_power(
        std::move(base), exponent, infinity, T{0}, add, multiply
    );
}

}
