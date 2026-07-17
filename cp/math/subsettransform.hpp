#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n 2^n)。subset_zeta(values): values[S]をsum_{T subset S} values[T]へ変換する。
template <class T>
void subset_zeta(std::vector<T>& values) {
    assert(std::has_single_bit(values.size()));
    for (std::size_t bit = 1; bit < values.size(); bit <<= 1) {
        for (std::size_t mask = 0; mask < values.size(); ++mask) {
            if (mask & bit) values[mask] += values[mask ^ bit];
        }
    }
}

/// O(n 2^n)。subset_mobius(values): subset_zetaの逆変換を行う。
template <class T>
void subset_mobius(std::vector<T>& values) {
    assert(std::has_single_bit(values.size()));
    for (std::size_t bit = 1; bit < values.size(); bit <<= 1) {
        for (std::size_t mask = 0; mask < values.size(); ++mask) {
            if (mask & bit) values[mask] -= values[mask ^ bit];
        }
    }
}

/// O(n 2^n)。superset_zeta(values): values[S]をsum_{T superset S} values[T]へ変換する。
template <class T>
void superset_zeta(std::vector<T>& values) {
    assert(std::has_single_bit(values.size()));
    for (std::size_t bit = 1; bit < values.size(); bit <<= 1) {
        for (std::size_t mask = 0; mask < values.size(); ++mask) {
            if (!(mask & bit)) values[mask] += values[mask | bit];
        }
    }
}

/// O(n 2^n)。superset_mobius(values): superset_zetaの逆変換を行う。
template <class T>
void superset_mobius(std::vector<T>& values) {
    assert(std::has_single_bit(values.size()));
    for (std::size_t bit = 1; bit < values.size(); bit <<= 1) {
        for (std::size_t mask = 0; mask < values.size(); ++mask) {
            if (!(mask & bit)) values[mask] -= values[mask | bit];
        }
    }
}

/// O(n^2 2^n)。subset_convolution(left, right)[S]=sum_{T subset S} left[T]right[S-T]。
template <class T>
std::vector<T> subset_convolution(
    const std::vector<T>& left, const std::vector<T>& right) {
    assert(left.size() == right.size());
    assert(std::has_single_bit(left.size()));
    const int bits = static_cast<int>(std::bit_width(left.size()) - 1);
    const int count = static_cast<int>(left.size());
    std::vector<std::vector<T>> ranked_left(bits + 1, std::vector<T>(count));
    std::vector<std::vector<T>> ranked_right(bits + 1, std::vector<T>(count));
    for (int mask = 0; mask < count; ++mask) {
        const int rank = std::popcount(static_cast<unsigned int>(mask));
        ranked_left[rank][mask] = left[mask];
        ranked_right[rank][mask] = right[mask];
    }
    for (int rank = 0; rank <= bits; ++rank) {
        subset_zeta(ranked_left[rank]);
        subset_zeta(ranked_right[rank]);
    }
    std::vector<std::vector<T>> ranked_result(bits + 1, std::vector<T>(count));
    for (int rank = 0; rank <= bits; ++rank) {
        for (int left_rank = 0; left_rank <= rank; ++left_rank) {
            for (int mask = 0; mask < count; ++mask) {
                ranked_result[rank][mask] +=
                    ranked_left[left_rank][mask] * ranked_right[rank - left_rank][mask];
            }
        }
        subset_mobius(ranked_result[rank]);
    }
    std::vector<T> result(count);
    for (int mask = 0; mask < count; ++mask) {
        result[mask] = ranked_result[std::popcount(static_cast<unsigned int>(mask))][mask];
    }
    return result;
}

/// O(n^2 2^n)。values[0]=1のsubset convolutionをexponent回行った列を返す。
template <class T>
std::vector<T> subset_convolution_unit_power(
    const std::vector<T>& values, long long exponent) {
    assert(std::has_single_bit(values.size()));
    assert(exponent >= 0);
    assert(values[0] == T{1});
    const int bits = static_cast<int>(std::bit_width(values.size()) - 1);
    const int count = static_cast<int>(values.size());
    std::vector<std::vector<T>> ranked(bits + 1, std::vector<T>(count));
    for (int mask = 0; mask < count; ++mask) {
        ranked[std::popcount(static_cast<unsigned int>(mask))][mask] = values[mask];
    }
    for (int rank = 0; rank <= bits; ++rank) subset_zeta(ranked[rank]);

    std::vector<std::vector<T>> powered(bits + 1, std::vector<T>(count));
    for (int mask = 0; mask < count; ++mask) {
        powered[0][mask] = T{1};
        for (int rank = 1; rank <= bits; ++rank) {
            T value{};
            for (int part = 1; part <= rank; ++part) {
                value += (T{exponent + 1} * T{part} - T{rank}) *
                         ranked[part][mask] * powered[rank - part][mask];
            }
            powered[rank][mask] = value / T{rank};
        }
    }
    for (int rank = 0; rank <= bits; ++rank) subset_mobius(powered[rank]);

    std::vector<T> result(count);
    for (int mask = 0; mask < count; ++mask) {
        result[mask] = powered[std::popcount(static_cast<unsigned int>(mask))][mask];
    }
    return result;
}

/// O(n 2^n)。all[S]=Sの全条件を満たす対象数から、ちょうどk条件を満たす対象数を返す。
template <class T>
std::vector<T> exact_match_counts(std::vector<T> all) {
    assert(std::has_single_bit(all.size()));
    superset_mobius(all);
    const int conditions = static_cast<int>(std::bit_width(all.size()) - 1);
    std::vector<T> result(conditions + 1);
    for (std::size_t mask = 0; mask < all.size(); ++mask) {
        result[std::popcount(mask)] += all[mask];
    }
    return result;
}

/// O(n 2^n)。1以上limit以下で、divisorsのうちちょうどk個の倍数である整数の個数を返す。
inline std::vector<long long> divisibility_match_counts(
    long long limit, const std::vector<long long>& divisors) {
    assert(limit >= 0);
    assert(divisors.size() < std::numeric_limits<std::size_t>::digits);
    for (const long long divisor : divisors) assert(divisor > 0);

    const std::size_t count = std::size_t{1} << divisors.size();
    std::vector<long long> lcm(count);
    std::vector<long long> all(count);
    lcm[0] = 1;
    all[0] = limit;
    for (std::size_t mask = 1; mask < count; ++mask) {
        const std::size_t bit = std::countr_zero(mask);
        const std::size_t previous = mask & (mask - 1);
        if (lcm[previous] == 0) continue;
        const long long divisor = divisors[bit];
        const long long reduced = lcm[previous] / std::gcd(lcm[previous], divisor);
        if (reduced > limit / divisor) continue;
        lcm[mask] = reduced * divisor;
        all[mask] = limit / lcm[mask];
    }
    return exact_match_counts(std::move(all));
}

}
