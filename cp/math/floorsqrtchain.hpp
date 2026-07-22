#pragma once
#include <bits/stdc++.h>
#include "math/math.hpp"

namespace poe {

/// floorsqrtchaincounter(max_value): O(max_value^(1/4))時間・空間で前計算する。
struct floorsqrtchaincounter {
private:
    using u64 = unsigned long long;
    using u128 = unsigned __int128;

    u64 max_value_;
    std::vector<u64> prefix_count_;
    std::vector<u128> prefix_square_count_;

public:
    explicit floorsqrtchaincounter(u64 max_value) : max_value_(max_value) {
        assert(max_value >= 1);
        const u64 limit = floor_sqrt(floor_sqrt(max_value));
        std::vector<u64> count(limit + 1);
        prefix_count_.assign(limit + 1, 0);
        prefix_square_count_.assign(limit + 1, 0);
        count[1] = 1;
        for (u64 x = 1; x <= limit; ++x) {
            if (x >= 2) count[x] = prefix_count_[floor_sqrt(x)];
            prefix_count_[x] = prefix_count_[x - 1] + count[x];
            prefix_square_count_[x] =
                prefix_square_count_[x - 1] + static_cast<u128>(x) * x * count[x];
        }
    }

    /// O(1)。xから始め、次を[1,floor(sqrt(現在値))]から選ぶ無限列の個数を返す。
    long long count(unsigned long long x) const {
        assert(1 <= x && x <= max_value_);
        const u64 root = floor_sqrt(x);
        const u64 limit = floor_sqrt(root);
        const u128 answer = static_cast<u128>(root + 1) * prefix_count_[limit]
                          - prefix_square_count_[limit];
        assert(answer <= static_cast<u128>(std::numeric_limits<long long>::max()));
        return static_cast<long long>(answer);
    }

    /// O(1)。count(x)と同じ。
    long long operator()(unsigned long long x) const { return count(x); }
};

}
