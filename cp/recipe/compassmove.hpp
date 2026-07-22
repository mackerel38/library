#pragma once
#include <bits/stdc++.h>
#include "geometry/latticemove.hpp"

namespace poe {

/// compassmoves(s).distance(x,y): sで許可した東・北東・北・北西・西・南西・南・南東移動の最短手数。
struct compassmoves {
    static inline constexpr std::array<point<long long>, 8> directions{{
        {1, 0}, {1, 1}, {0, 1}, {-1, 1},
        {-1, 0}, {-1, -1}, {0, -1}, {1, -1},
    }};

    unsigned int mask = 0;

    /// O(8)。'1'を許可、'0'を禁止とする長さ8の文字列から作る。
    explicit compassmoves(std::string_view allowed) {
        assert(allowed.size() == directions.size());
        for (int index = 0; index < 8; ++index) {
            assert(allowed[index] == '0' || allowed[index] == '1');
            if (allowed[index] == '1') mask |= 1U << index;
        }
    }

    /// O(1)。bit iが方向iを許可するmaskから作る。
    explicit compassmoves(unsigned int allowed_mask) : mask(allowed_mask) {
        assert((mask >> 8) == 0);
    }

    /// O(1)。0-indexed方向indexが許可されているか返す。
    bool allowed(int index) const {
        assert(0 <= index && index < 8);
        return mask >> index & 1U;
    }

    /// O(1)。原点から(x,y)への最小手数を返し、到達不能ならnullopt。
    std::optional<long long> distance(long long x, long long y) const {
        const point<long long> target{x, y};
        if (target == point<long long>{}) return 0;
        std::vector<point<long long>> enabled;
        std::vector<point<long long>> diagonal;
        for (int index = 0; index < 8; ++index) {
            if (!allowed(index)) continue;
            enabled.push_back(directions[index]);
            if (index % 2 == 1) diagonal.push_back(directions[index]);
        }
        std::optional<long long> answer = minimum_two_kind_lattice_moves(target, enabled);
        const auto relax = [&](std::optional<long long> candidate) {
            if (candidate && *candidate < std::numeric_limits<long long>::max()) {
                ++*candidate;
                if (!answer || *candidate < *answer) answer = candidate;
            }
        };
        for (int index = 0; index < 8; index += 2) {
            if (allowed(index)) {
                relax(minimum_two_kind_lattice_moves(target - directions[index], diagonal));
            }
        }
        return answer;
    }

    /// O(1)。原点からtargetへの最小手数を返し、到達不能ならnullopt。
    std::optional<long long> distance(const point<long long>& target) const {
        return distance(target.x, target.y);
    }
};

}
