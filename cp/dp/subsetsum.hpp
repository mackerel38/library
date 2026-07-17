#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n * limit / 64)。subsetsum(values, limit): 0/1部分和で作れる0以上limit以下の和を返す。
inline std::vector<bool> subsetsum(const std::vector<int>& values, int limit) {
    assert(limit >= 0);
    const int words = (limit + 64) / 64;
    std::vector<std::uint64_t> bits(static_cast<std::size_t>(words));
    bits[0] = 1;
    for (const int value : values) {
        assert(value >= 0);
        if (value > limit) continue;
        const int word_shift = value >> 6;
        const int bit_shift = value & 63;
        for (int destination = words - 1; destination >= word_shift; --destination) {
            std::uint64_t shifted = bits[destination - word_shift] << bit_shift;
            if (bit_shift != 0 && destination > word_shift) {
                shifted |= bits[destination - word_shift - 1] >> (64 - bit_shift);
            }
            bits[destination] |= shifted;
        }
    }
    std::vector<bool> result(static_cast<std::size_t>(limit) + 1);
    for (int sum = 0; sum <= limit; ++sum) {
        result[sum] = (bits[sum >> 6] >> (sum & 63)) & 1U;
    }
    return result;
}

/// O(n * limit)。bounded_subsetsum(value, count, limit): 個数制限付き部分和の可否を返す。
inline std::vector<bool> bounded_subsetsum(
    const std::vector<int>& values,
    const std::vector<int>& counts,
    int limit
) {
    assert(values.size() == counts.size());
    assert(limit >= 0);
    std::vector<int> remaining(static_cast<std::size_t>(limit) + 1, -1);
    remaining[0] = 0;
    for (std::size_t item = 0; item < values.size(); ++item) {
        assert(values[item] > 0 && counts[item] >= 0);
        for (int sum = 0; sum <= limit; ++sum) {
            if (remaining[sum] >= 0) {
                remaining[sum] = counts[item];
            } else if (sum < values[item] || remaining[sum - values[item]] <= 0) {
                remaining[sum] = -1;
            } else {
                remaining[sum] = remaining[sum - values[item]] - 1;
            }
        }
    }
    std::vector<bool> result(static_cast<std::size_t>(limit) + 1);
    for (int sum = 0; sum <= limit; ++sum) result[sum] = remaining[sum] >= 0;
    return result;
}

/// subset_sum_less_by_sizeの結果。count[k]はk個選ぶ組数、sum[k]はその部分集合和の総計。
struct subsetsumstatistics {
    std::vector<unsigned long long> count;
    std::vector<__int128_t> sum;
};

/// O(n 2^(n/2))時間・O(2^(n/2))領域。要素数別に部分集合和がlimit未満の個数と和を返す。
inline subsetsumstatistics subset_sum_less_by_size(
    const std::vector<long long>& values,
    long long limit
) {
    const int n = static_cast<int>(values.size());
    assert(n <= 40);
    for (long long value : values) assert(value >= 0);
    const int middle = n / 2;

    auto enumerate = [&](int left, int right) {
        const int length = right - left;
        std::vector<std::vector<long long>> result(length + 1);
        std::vector<long long> sums(std::size_t{1} << length);
        for (std::size_t mask = 1; mask < sums.size(); ++mask) {
            const int bit = std::countr_zero(mask);
            const std::size_t previous = mask ^ (std::size_t{1} << bit);
            const __int128_t next = __int128_t{sums[previous]} + values[left + bit];
            assert(next <= std::numeric_limits<long long>::max());
            sums[mask] = static_cast<long long>(next);
        }
        for (std::size_t mask = 0; mask < sums.size(); ++mask) {
            result[std::popcount(mask)].push_back(sums[mask]);
        }
        return result;
    };

    auto first = enumerate(0, middle);
    auto second = enumerate(middle, n);
    std::vector<std::vector<__int128_t>> prefix(second.size());
    for (int size = 0; size < static_cast<int>(second.size()); ++size) {
        std::ranges::sort(second[size]);
        prefix[size].resize(second[size].size() + 1);
        for (int index = 0; index < static_cast<int>(second[size].size()); ++index) {
            prefix[size][index + 1] = prefix[size][index] + second[size][index];
        }
    }

    subsetsumstatistics result{
        std::vector<unsigned long long>(n + 1),
        std::vector<__int128_t>(n + 1)
    };
    for (int first_size = 0; first_size < static_cast<int>(first.size()); ++first_size) {
        for (long long first_sum : first[first_size]) {
            for (int second_size = 0; second_size < static_cast<int>(second.size()); ++second_size) {
                const __int128_t threshold = __int128_t{limit} - first_sum;
                std::size_t count;
                if (threshold <= std::numeric_limits<long long>::min()) {
                    count = 0;
                } else if (threshold > std::numeric_limits<long long>::max()) {
                    count = second[second_size].size();
                } else {
                    count = std::lower_bound(second[second_size].begin(), second[second_size].end(),
                                             static_cast<long long>(threshold)) -
                            second[second_size].begin();
                }
                const int total_size = first_size + second_size;
                result.count[total_size] += count;
                result.sum[total_size] +=
                    __int128_t{first_sum} * count + prefix[second_size][count];
            }
        }
    }
    return result;
}

/// 非隣接部分集合和列挙。last_selectedは最後の要素を選ぶ場合、allは全場合の剰余。
template <class T>
struct nonadjacentsubsetsums {
    std::vector<T> last_selected;
    std::vector<T> all;
};

/// O(F_n)。path上で隣接要素を同時に選ばない部分集合和をmoduloで割った剰余を列挙する。
template <class T>
nonadjacentsubsetsums<T> nonadjacent_subset_sums(
    const std::vector<T>& values,
    T modulo
) {
    assert(modulo > T{});
    std::vector<T> selected;
    std::vector<T> unselected{T{}};
    for (const T& value : values) {
        assert(T{} <= value && value < modulo);
        std::swap(selected, unselected);
        unselected.insert(unselected.end(), selected.begin(), selected.end());
        for (T& sum : selected) {
            if constexpr (std::is_integral_v<T> && sizeof(T) <= 8) {
                sum = static_cast<T>(
                    (static_cast<unsigned __int128>(sum) + value) % modulo
                );
            } else {
                sum = (sum + value) % modulo;
            }
        }
    }
    nonadjacentsubsetsums<T> result;
    result.last_selected = std::move(selected);
    result.all = result.last_selected;
    result.all.insert(result.all.end(), unselected.begin(), unselected.end());
    return result;
}

/// O(F_(n/2) log F_(n/2))。非隣接部分集合のうち和≡target(mod modulo)となる個数を返す。
template <class T>
unsigned long long count_nonadjacent_subsets_mod(
    const std::vector<T>& values,
    T modulo,
    T target = T{}
) {
    assert(modulo > T{});
    assert(T{} <= target && target < modulo);
    const int middle = static_cast<int>(values.size()) / 2;
    const std::vector<T> left_values(values.begin(), values.begin() + middle);
    std::vector<T> right_values(values.begin() + middle, values.end());
    std::ranges::reverse(right_values);
    auto left = nonadjacent_subset_sums(left_values, modulo);
    auto right = nonadjacent_subset_sums(right_values, modulo);
    std::ranges::sort(right.all);
    std::ranges::sort(right.last_selected);
    auto count_pairs = [&](const std::vector<T>& first, const std::vector<T>& second) {
        unsigned long long result = 0;
        for (const T& sum : first) {
            const T needed = target >= sum ? target - sum : modulo - (sum - target);
            const auto [begin, end] = std::ranges::equal_range(second, needed);
            result += static_cast<unsigned long long>(end - begin);
        }
        return result;
    };
    return count_pairs(left.all, right.all)
         - count_pairs(left.last_selected, right.last_selected);
}

}
