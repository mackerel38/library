#pragma once
#include <bits/stdc++.h>
#include "math/combination.hpp"

namespace poe {

/// O(n^3 log(maximum+target))。0..maximumから選ぶ長さ0..nの順序付きtupleでXORがtargetとなる個数を返す。
template<class T>
std::vector<T> bounded_xor_tuple_counts(
    int maximum_length,
    unsigned long long maximum,
    unsigned long long target
) {
    assert(maximum_length >= 0);
    const int bits = std::max({1, static_cast<int>(std::bit_width(maximum)),
                              static_cast<int>(std::bit_width(target))});
    combination<T> combinations(maximum_length);
    std::vector<T> power_two(maximum_length + 1, T{1});
    for (int count = 1; count <= maximum_length; ++count) {
        power_two[count] = power_two[count - 1] * T{2};
    }

    std::vector<T> answer(maximum_length + 1);
    for (int length = 0; length <= maximum_length; ++length) {
        std::vector<T> dp(length + 1), next(length + 1);
        dp[0] = T{1};
        for (int bit = bits - 1; bit >= 0; --bit) {
            std::fill(next.begin(), next.end(), T{});
            const int limit_bit = maximum >> bit & 1ULL;
            const int target_bit = target >> bit & 1ULL;
            for (int loose = 0; loose <= length; ++loose) {
                if (dp[loose] == T{}) continue;
                const T loose_choices = loose == 0 ? T{1} : power_two[loose - 1];
                if (limit_bit == 0) {
                    if (loose > 0 || target_bit == 0) {
                        next[loose] += dp[loose] * loose_choices;
                    }
                    continue;
                }
                const int tight = length - loose;
                for (int tight_ones = 0; tight_ones <= tight; ++tight_ones) {
                    if (loose == 0 && (tight_ones & 1) != target_bit) continue;
                    next[length - tight_ones] += dp[loose] * loose_choices
                        * combinations.choose(tight, tight_ones);
                }
            }
            dp.swap(next);
        }
        for (const T& value : dp) answer[length] += value;
    }
    return answer;
}

/// O(n^3)時間・O(n^2)空間。XORが固定値となる重複可tuple数から、全要素が相異なるtuple数を返す。
template<class T>
std::vector<T> distinct_xor_tuple_counts(
    const std::vector<T>& tuple_counts,
    long long alphabet_size
) {
    assert(!tuple_counts.empty() && alphabet_size >= 0);
    const int maximum_length = static_cast<int>(tuple_counts.size()) - 1;
    combination<T> combinations(maximum_length);

    std::vector odd(maximum_length + 1, std::vector<T>(maximum_length + 1));
    std::vector even(maximum_length + 1, std::vector<T>(maximum_length + 1));
    odd[0][0] = even[0][0] = T{1};
    for (int size = 1; size <= maximum_length; ++size) {
        for (int groups = 1; groups <= size; ++groups) {
            for (int block = 1; block <= size; block += 2) {
                odd[size][groups] += combinations.choose(size - 1, block - 1)
                    * odd[size - block][groups - 1];
            }
            for (int block = 2; block <= size; block += 2) {
                even[size][groups] += combinations.choose(size - 1, block - 1)
                    * even[size - block][groups - 1];
            }
        }
    }

    const int maximum_distinct = static_cast<int>(
        std::min<long long>(maximum_length, alphabet_size)
    );
    std::vector falling(maximum_distinct + 1, std::vector<T>(maximum_length + 1));
    for (int used = 0; used <= maximum_distinct; ++used) {
        falling[used][0] = T{1};
        for (int count = 1; count <= maximum_length; ++count) {
            if (count <= alphabet_size - used) {
                falling[used][count] = falling[used][count - 1]
                    * T{alphabet_size - used - count + 1};
            }
        }
    }
    std::vector even_assignments(maximum_length + 1,
        std::vector<T>(maximum_distinct + 1));
    for (int size = 0; size <= maximum_length; ++size) {
        for (int used = 0; used <= maximum_distinct; ++used) {
            for (int groups = 0; groups <= size; ++groups) {
                even_assignments[size][used] += even[size][groups] * falling[used][groups];
            }
        }
    }

    std::vector<T> distinct(maximum_length + 1);
    for (int length = 0; length <= maximum_length; ++length) {
        if (length > alphabet_size) continue;
        T value = tuple_counts[length];
        for (int odd_elements = 0; odd_elements <= length; ++odd_elements) {
            const int last_groups = std::min({length - 1, odd_elements, maximum_distinct});
            for (int odd_groups = 0; odd_groups <= last_groups; ++odd_groups) {
                value -= combinations.choose(length, odd_elements)
                    * odd[odd_elements][odd_groups]
                    * distinct[odd_groups]
                    * even_assignments[length - odd_elements][odd_groups];
            }
        }
        distinct[length] = value;
    }
    return distinct;
}

/// O(n)。distinct_xor_tuple_countsの結果から、長さnの非減少XOR列、すなわちmultiset数を返す。
template<class T>
T xor_multiset_count(
    int length,
    long long alphabet_size,
    const std::vector<T>& distinct_counts
) {
    assert(0 <= length && length < static_cast<int>(distinct_counts.size()));
    assert(alphabet_size > 0);
    combination<T> combinations(length);
    T answer{};
    for (int pairs = 0; 2 * pairs <= length; ++pairs) {
        const int distinct = length - 2 * pairs;
        answer += distinct_counts[distinct] / combinations.factorial(distinct)
            * combinations.choose_large(alphabet_size + pairs - 1, pairs);
    }
    return answer;
}

}
