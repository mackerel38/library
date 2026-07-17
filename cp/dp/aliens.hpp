#pragma once
#include <bits/stdc++.h>

namespace poe {

/// Aliens trickのペナルティ探索結果。
template <class T>
struct aliens_result {
    T value;
    T penalty;
    int count;
};

/// O(log(high-low) * solve)。最小化DPで区切り数targetの値をAliens trickで復元する。
/// solve(p)は「元費用+p*個数」の最小値と、その中で最小の個数を返す。
/// penalty増加に対してcountが広義単調減少し、離散凸性によりvalue-penalty*targetが成立すること。
template <class T, class Solve>
aliens_result<T> aliens_min(T low, T high, int target, Solve solve) {
    assert(low <= high && target >= 0);
    while (low < high) {
        const T middle = low + (high - low) / 2;
        const auto [value, count] = solve(middle);
        (void)value;
        if (count <= target) high = middle;
        else low = middle + 1;
    }
    const auto [penalized, count] = solve(low);
    return {penalized - low * T{target}, low, count};
}

/// O(log(high-low) * solve)。最大化DPで個数targetの値をAliens trickで復元する。
/// solve(p)は「元価値-p*個数」の最大値と、その中で最小の個数を返す。
/// penalty増加に対してcountが広義単調減少し、離散凹性によりvalue+penalty*targetが成立すること。
template <class T, class Solve>
aliens_result<T> aliens_max(T low, T high, int target, Solve solve) {
    assert(low <= high && target >= 0);
    while (low < high) {
        const T middle = low + (high - low) / 2;
        const auto [value, count] = solve(middle);
        (void)value;
        if (count <= target) high = middle;
        else low = middle + 1;
    }
    const auto [penalized, count] = solve(low);
    return {penalized + low * T{target}, low, count};
}

}
