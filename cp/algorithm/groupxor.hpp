#pragma once
#include <bits/stdc++.h>
#include "structure/xortrie.hpp"

namespace poe {

namespace detail {

inline std::vector<std::pair<unsigned long long, unsigned long long>> subset_products(
    const std::vector<unsigned long long>& factors,
    int begin,
    int end
) {
    std::vector<std::pair<unsigned long long, unsigned long long>> result{{1, 0}};
    for (int index = begin; index < end; ++index) {
        const std::size_t size = result.size();
        for (std::size_t i = 0; i < size; ++i) {
            result.emplace_back(result[i].first * factors[index],
                                result[i].second | (1ULL << (index - begin)));
        }
    }
    return result;
}

template<std::unsigned_integral UInt>
std::vector<UInt> enumerate_group_xors(
    const std::vector<std::vector<UInt>>& groups,
    const std::vector<int>& selected,
    UInt initial
) {
    std::vector<UInt> values{initial};
    for (const int group : selected) {
        std::vector<UInt> next;
        next.reserve(values.size() * groups[group].size());
        for (const UInt prefix : values) {
            for (const UInt value : groups[group]) next.push_back(prefix ^ value);
        }
        values = std::move(next);
    }
    return values;
}

}

/// O(2^(g/2)+(L+R) bits)時間・O(2^(g/2)+(L+R) bits)領域。各groupから一つ選んだXORのk番目に大きい値を返す。
template<std::unsigned_integral UInt, int Bits = std::numeric_limits<UInt>::digits>
UInt kth_largest_group_xor(
    const std::vector<std::vector<UInt>>& groups,
    unsigned long long k
) {
    static_assert(1 <= Bits && Bits <= std::numeric_limits<UInt>::digits);
    UInt fixed = 0;
    std::vector<int> variable;
    std::vector<unsigned long long> factors;
    unsigned long long total = 1;
    for (int group = 0; group < static_cast<int>(groups.size()); ++group) {
        assert(!groups[group].empty());
        if (groups[group].size() == 1) {
            fixed ^= groups[group][0];
        } else {
            variable.push_back(group);
            factors.push_back(groups[group].size());
            assert(total <= std::numeric_limits<unsigned long long>::max() / groups[group].size());
            total *= groups[group].size();
        }
    }
    assert(1 <= k && k <= total);
    assert(variable.size() <= 62);

    const int middle = static_cast<int>(variable.size()) / 2;
    auto first = detail::subset_products(factors, 0, middle);
    auto second = detail::subset_products(factors, middle, static_cast<int>(variable.size()));
    std::ranges::sort(second, {}, &std::pair<unsigned long long, unsigned long long>::first);

    unsigned long long best_cost = total;
    unsigned long long first_mask = 0;
    unsigned long long second_mask = 0;
    const long double target = std::sqrt(static_cast<long double>(total));
    for (const auto [product, mask] : first) {
        const unsigned long long wanted = static_cast<unsigned long long>(target / product);
        const auto iterator = std::ranges::lower_bound(second, wanted, {},
            &std::pair<unsigned long long, unsigned long long>::first);
        for (int offset : {-1, 0}) {
            if (iterator == second.begin() && offset == -1) continue;
            if (iterator == second.end() && offset == 0) continue;
            const auto candidate = iterator + offset;
            const unsigned long long side = product * candidate->first;
            const unsigned long long cost = std::max(side, total / side);
            if (cost < best_cost) {
                best_cost = cost;
                first_mask = mask;
                second_mask = candidate->second;
            }
        }
    }

    std::vector<int> left_groups;
    std::vector<int> right_groups;
    for (int index = 0; index < static_cast<int>(variable.size()); ++index) {
        const bool selected = index < middle
            ? (first_mask >> index & 1)
            : (second_mask >> (index - middle) & 1);
        (selected ? left_groups : right_groups).push_back(variable[index]);
    }
    const auto left = detail::enumerate_group_xors(groups, left_groups, fixed);
    const auto right = detail::enumerate_group_xors(groups, right_groups, UInt{});
    return kth_largest_cross_xor<UInt, Bits>(left, right, k);
}

}
