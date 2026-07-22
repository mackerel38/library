#pragma once
#include <bits/stdc++.h>
#include "math/convolution.hpp"

namespace poe {

/// O(KL(log L+log K+log exponent))時間・O(KL)領域。
/// sum添字は加算、mask添字はXORする二次元分布のexponent乗を返す。
template <int mod>
std::vector<std::vector<staticmodint<mod>>> sum_xor_convolution_power(
    const std::vector<std::vector<staticmodint<mod>>>& base,
    long long exponent
) {
    using mint = staticmodint<mod>;
    assert(exponent >= 0 && !base.empty() && !base.front().empty());
    const int masks = static_cast<int>(base.front().size());
    assert(std::has_single_bit(static_cast<unsigned int>(masks)));
    for (const auto& row : base) assert(static_cast<int>(row.size()) == masks);
    if (exponent == 0) {
        std::vector<std::vector<mint>> result(1, std::vector<mint>(masks));
        result[0][0] = 1;
        return result;
    }

    const __int128 result_size_wide =
        static_cast<__int128>(base.size() - 1) * exponent + 1;
    assert(result_size_wide <= std::numeric_limits<int>::max());
    const int result_size = static_cast<int>(result_size_wide);
    const int transform_size = static_cast<int>(std::bit_ceil(
        static_cast<unsigned int>(result_size)
    ));
    assert((mod - 1) % transform_size == 0);

    std::vector<std::vector<mint>> transformed(
        masks, std::vector<mint>(transform_size)
    );
    for (int sum = 0; sum < static_cast<int>(base.size()); ++sum) {
        for (int mask = 0; mask < masks; ++mask) transformed[mask][sum] = base[sum][mask];
    }

    for (int bit = 1; bit < masks; bit <<= 1) {
        for (int mask = 0; mask < masks; ++mask) {
            if (mask & bit) continue;
            for (int sum = 0; sum < transform_size; ++sum) {
                const mint left = transformed[mask][sum];
                const mint right = transformed[mask | bit][sum];
                transformed[mask][sum] = left + right;
                transformed[mask | bit][sum] = left - right;
            }
        }
    }
    for (auto& polynomial : transformed) {
        detail::ntt(polynomial, false);
        for (mint& value : polynomial) value = value.pow(exponent);
        detail::ntt(polynomial, true);
    }
    for (int bit = 1; bit < masks; bit <<= 1) {
        for (int mask = 0; mask < masks; ++mask) {
            if (mask & bit) continue;
            for (int sum = 0; sum < result_size; ++sum) {
                const mint left = transformed[mask][sum];
                const mint right = transformed[mask | bit][sum];
                transformed[mask][sum] = left + right;
                transformed[mask | bit][sum] = left - right;
            }
        }
    }
    const mint inverse_masks = mint{masks}.inv();
    std::vector<std::vector<mint>> result(
        result_size, std::vector<mint>(masks)
    );
    for (int sum = 0; sum < result_size; ++sum) {
        for (int mask = 0; mask < masks; ++mask) {
            result[sum][mask] = transformed[mask][sum] * inverse_masks;
        }
    }
    return result;
}

}
