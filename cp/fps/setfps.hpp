#pragma once
#include <bits/stdc++.h>
#include "math/modint.hpp"

namespace poe {

/// O(3^n)。集合冪級数f（f[0]=0）のexpを返す。配列長は2冪。
template<int mod>
std::vector<staticmodint<mod>> setfps_exp(const std::vector<staticmodint<mod>>& f) {
    using mint = staticmodint<mod>;
    const int size = static_cast<int>(f.size());
    assert(size > 0 && std::has_single_bit(static_cast<unsigned int>(size)) && f[0] == mint{});
    std::vector<mint> result(size);
    result[0] = 1;
    for (int mask = 1; mask < size; ++mask) {
        const int pivot = std::countr_zero(static_cast<unsigned int>(mask));
        const int bit = 1 << pivot;
        for (int subset = mask; subset; subset = (subset - 1) & mask) {
            if (subset & bit) result[mask] += f[subset] * result[mask ^ subset];
        }
    }
    return result;
}

/// O(3^n)。集合冪級数f（f[0]=1）のlogを返す。配列長は2冪。
template<int mod>
std::vector<staticmodint<mod>> setfps_log(const std::vector<staticmodint<mod>>& f) {
    using mint = staticmodint<mod>;
    const int size = static_cast<int>(f.size());
    assert(size > 0 && std::has_single_bit(static_cast<unsigned int>(size)) && f[0] == mint{1});
    std::vector<mint> result(size);
    for (int mask = 1; mask < size; ++mask) {
        result[mask] = f[mask];
        const int pivot = std::countr_zero(static_cast<unsigned int>(mask));
        const int bit = 1 << pivot;
        for (int subset = (mask - 1) & mask; subset; subset = (subset - 1) & mask) {
            if (subset & bit) result[mask] -= result[subset] * f[mask ^ subset];
        }
    }
    return result;
}

/// O(3^(n-1))。vertexを含む係数だけを、その偏微分SPSのexpまたはlogで置き換える。
template<int mod>
void setfps_articulation_transform(
    std::vector<staticmodint<mod>>& f,
    int vertex,
    bool exponential
) {
    using mint = staticmodint<mod>;
    const int size = static_cast<int>(f.size());
    assert(size > 0 && std::has_single_bit(static_cast<unsigned int>(size)));
    const int vertices = std::countr_zero(static_cast<unsigned int>(size));
    assert(0 <= vertex && vertex < vertices);
    const int bit = 1 << vertex;
    std::vector<mint> slice(size / 2);
    for (int compressed = 0; compressed < size / 2; ++compressed) {
        const int low = compressed & (bit - 1);
        const int high = compressed ^ low;
        slice[compressed] = f[low | (high << 1) | bit];
    }
    slice = exponential ? setfps_exp<mod>(slice) : setfps_log<mod>(slice);
    for (int compressed = 0; compressed < size / 2; ++compressed) {
        const int low = compressed & (bit - 1);
        const int high = compressed ^ low;
        f[low | (high << 1) | bit] = slice[compressed];
    }
}

}
