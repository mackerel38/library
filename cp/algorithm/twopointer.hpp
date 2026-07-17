#pragma once
#include <bits/stdc++.h>

namespace poe {

namespace detail {

struct inversionfenwick {
    explicit inversionfenwick(int size) : data(size + 1) {}
    void add(int index, int value) {
        for (++index; index < static_cast<int>(data.size()); index += index & -index)
            data[index] += value;
    }
    int sum(int right) const {
        int result = 0;
        for (; right > 0; right -= right & -right) result += data[right];
        return result;
    }
    std::vector<int> data;
};

template<class T>
long long count_subarrays_sum_at_most_nonnegative(
    const std::vector<T>& values,
    long long limit
) {
    if (limit < 0) return 0;
    long long answer = 0;
    long long sum = 0;
    int left = 0;
    for (int right = 0; right < static_cast<int>(values.size()); ++right) {
        assert(values[right] >= T{});
        sum += static_cast<long long>(values[right]);
        while (left <= right && sum > limit) {
            sum -= static_cast<long long>(values[left++]);
        }
        answer += right - left + 1;
    }
    return answer;
}

}

/// O(n log n)。転倒数がlimit以下である空でない部分配列の個数を返す。
template<class T>
long long count_subarrays_inversions_at_most(
    const std::vector<T>& values,
    long long limit
) {
    if (limit < 0) return 0;
    std::vector<T> sorted = values;
    std::ranges::sort(sorted);
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
    detail::inversionfenwick frequency(static_cast<int>(sorted.size()));
    std::vector<int> rank(values.size());
    for (int i = 0; i < static_cast<int>(values.size()); ++i) {
        rank[i] = static_cast<int>(std::lower_bound(sorted.begin(), sorted.end(), values[i])
                                   - sorted.begin());
    }
    long long answer = 0;
    long long inversions = 0;
    int left = 0;
    for (int right = 0; right < static_cast<int>(values.size()); ++right) {
        inversions += (right - left) - frequency.sum(rank[right] + 1);
        frequency.add(rank[right], 1);
        while (inversions > limit) {
            frequency.add(rank[left], -1);
            inversions -= frequency.sum(rank[left]);
            ++left;
        }
        answer += right - left + 1;
    }
    return answer;
}

/// O(n log n)。転倒数がtargetと等しい空でない部分配列の個数を返す。
template<class T>
long long count_subarrays_inversions(
    const std::vector<T>& values,
    long long target
) {
    return count_subarrays_inversions_at_most(values, target)
         - count_subarrays_inversions_at_most(values, target - 1);
}

/// O(n)。非負整数列で和がtargetとなる空でない部分配列の個数を返す。
template<class T>
long long count_subarrays_sum_nonnegative(
    const std::vector<T>& values,
    long long target
) {
    if (target < 0) return 0;
    return detail::count_subarrays_sum_at_most_nonnegative(values, target)
        - detail::count_subarrays_sum_at_most_nonnegative(values, target - 1);
}

/// O(min(h,w)^2 max(h,w))。非負整数行列で和がtargetとなる空でない長方形の個数を返す。
template<class T>
long long count_subrectangles_sum_nonnegative(
    const std::vector<std::vector<T>>& matrix,
    long long target
) {
    const int original_height = static_cast<int>(matrix.size());
    if (original_height == 0) return 0;
    const int original_width = static_cast<int>(matrix.front().size());
    for (const auto& row : matrix) {
        assert(static_cast<int>(row.size()) == original_width);
        for (const T& value : row) assert(value >= T{});
    }
    if (original_width == 0 || target < 0) return 0;

    const bool transpose = original_height > original_width;
    const int height = transpose ? original_width : original_height;
    const int width = transpose ? original_height : original_width;
    auto value = [&](int row, int column) -> const T& {
        return transpose ? matrix[column][row] : matrix[row][column];
    };

    long long answer = 0;
    std::vector<long long> column_sum(width);
    for (int top = 0; top < height; ++top) {
        std::fill(column_sum.begin(), column_sum.end(), 0);
        for (int bottom = top; bottom < height; ++bottom) {
            for (int column = 0; column < width; ++column) {
                column_sum[column] += static_cast<long long>(value(bottom, column));
            }
            answer += count_subarrays_sum_nonnegative(column_sum, target);
        }
    }
    return answer;
}

namespace detail {

template<class Value, class Predicate>
std::vector<int> frequency_boundaries(
    const std::vector<Value>& values,
    int threshold,
    Predicate predicate
) {
    const int size = static_cast<int>(values.size());
    std::vector<int> result(size, size + 1);
    int right = 0;
    int count = 0;
    for (int left = 0; left < size; ++left) {
        while (right < size && count < threshold) {
            count += static_cast<bool>(std::invoke(predicate, values[right++]));
        }
        if (count >= threshold) result[left] = right;
        if (right > left) count -= static_cast<bool>(std::invoke(predicate, values[left]));
    }
    return result;
}

}

/// O(n)。Pをminimum以上、Qをmaximum未満含む空でない部分配列数を返す。minimum,maximum>=1。
template<std::ranges::input_range Range, class FirstPredicate, class SecondPredicate>
long long count_subarrays_frequency_constraints(
    const Range& values,
    int minimum_first,
    int maximum_second,
    FirstPredicate first_predicate,
    SecondPredicate second_predicate
) {
    assert(minimum_first >= 1 && maximum_second >= 1);
    using value_type = std::ranges::range_value_t<Range>;
    const std::vector<value_type> stored(std::ranges::begin(values), std::ranges::end(values));
    const auto first = detail::frequency_boundaries(stored, minimum_first, first_predicate);
    const auto second = detail::frequency_boundaries(stored, maximum_second, second_predicate);
    long long answer = 0;
    for (int left = 0; left < static_cast<int>(stored.size()); ++left) {
        if (first[left] <= static_cast<int>(stored.size())) {
            answer += std::max(0, second[left] - first[left]);
        }
    }
    return answer;
}

/// 円環列の始点から和が閾値以上になる最短区間の長さと和。
template <class Sum>
struct circularthresholdwindow {
    int length;
    Sum sum;
};

/// O(n)。非負円環列の各始点から和がtarget以上になる最短prefixを返す。
template <class T, class U>
auto circular_threshold_windows(const std::vector<T>& values, U target) {
    using sum_type = std::common_type_t<T, U, long long>;
    assert(!values.empty() && target > U{});
    const int size = static_cast<int>(values.size());
    sum_type total = 0;
    for (const T& value : values) {
        assert(value >= T{});
        total += static_cast<sum_type>(value);
    }
    assert(total >= static_cast<sum_type>(target));
    std::vector<circularthresholdwindow<sum_type>> result(size);
    int right = 0;
    sum_type sum = 0;
    for (int left = 0; left < size; ++left) {
        while (right < left + size && sum < static_cast<sum_type>(target)) {
            sum += static_cast<sum_type>(values[right % size]);
            ++right;
        }
        assert(sum >= static_cast<sum_type>(target));
        result[left] = {right - left, sum};
        sum -= static_cast<sum_type>(values[left]);
    }
    return result;
}

}
