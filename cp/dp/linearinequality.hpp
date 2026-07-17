#pragma once
#include <bits/stdc++.h>
#include "math/convolution.hpp"

namespace poe {

/// 非負整数列xでsum(weights[i]*x[i])<=limitとなる個数を返す; sum(weights)=SとしてO(S log S log limit)。
template <int mod = 998244353>
staticmodint<mod> count_nonnegative_weighted_sum_at_most(
    const std::vector<int>& weights, unsigned long long limit) {
    using mint = staticmodint<mod>;
    int weight_sum = 0;
    for (int weight : weights) {
        assert(weight > 0);
        weight_sum += weight;
    }

    // 各変数をx=2q+r (r in {0,1})と分けたときのsum(weight*r)の分布。
    std::vector<mint> remainder(weight_sum + 1);
    remainder[0] = 1;
    int current_sum = 0;
    for (int weight : weights) {
        for (int sum = current_sum; sum >= 0; --sum) {
            remainder[sum + weight] += remainder[sum];
        }
        current_sum += weight;
    }

    std::vector<mint> coefficient{1};
    while (limit > 0) {
        const auto product = convolution(remainder, coefficient);
        const unsigned long long reach = std::min<unsigned long long>(
            limit, product.size() - 1);
        const std::size_t result_size = static_cast<std::size_t>(
            limit / 2 - (limit - reach) / 2 + 1);
        std::vector<mint> next(result_size);
        for (std::size_t index = 0; index <= reach; ++index) {
            const std::size_t bucket = static_cast<std::size_t>(
                limit / 2 - (limit - index) / 2);
            next[bucket] += product[index];
        }
        coefficient = std::move(next);
        limit /= 2;
    }
    return coefficient.front();
}

}
