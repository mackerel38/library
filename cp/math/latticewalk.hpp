#pragma once
#include <bits/stdc++.h>
#include "math/combination.hpp"
#include "math/modint.hpp"

namespace poe {

/// axiswalkcounter(max_steps): O(max_steps)時間・空間で軸方向単位walkを前計算する。
template<int mod>
struct axiswalkcounter {
    using mint = staticmodint<mod>;

    /// O(max_steps)。0以上max_steps以下の歩数に必要な二項係数を前計算する。
    explicit axiswalkcounter(int max_steps) : max_steps_(max_steps), combination_(max_steps) {
        assert(0 <= max_steps && max_steps < mod);
    }

    /// O(1)。一次元で原点からxへちょうどsteps回の+-1移動で着く経路数を返す。
    mint count(int steps, long long x) const {
        assert(0 <= steps && steps <= max_steps_);
        if (x < -static_cast<long long>(steps) || steps < x) return 0;
        const long long distance = x < 0 ? -x : x;
        if ((steps - distance) & 1) return 0;
        return combination_.choose(steps, static_cast<int>((steps + distance) / 2));
    }

    /// O(1)。二次元で原点から(x,y)へちょうどsteps回の軸方向単位移動で着く経路数を返す。
    mint count(int steps, long long x, long long y) const {
        assert(0 <= steps && steps <= max_steps_);
        if (
            x < -static_cast<long long>(steps) || steps < x
            || y < -static_cast<long long>(steps) || steps < y
        ) {
            return 0;
        }
        return count(steps, x + y) * count(steps, x - y);
    }

    /// O(steps)。三次元で原点から(x,y,z)へちょうどsteps回の軸方向単位移動で着く経路数を返す。
    mint count(int steps, long long x, long long y, long long z) const {
        assert(0 <= steps && steps <= max_steps_);
        if (
            x < -static_cast<long long>(steps) || steps < x
            || y < -static_cast<long long>(steps) || steps < y
            || z < -static_cast<long long>(steps) || steps < z
        ) {
            return 0;
        }
        mint answer = 0;
        for (int z_steps = 0; z_steps <= steps; ++z_steps) {
            answer += combination_.choose(steps, z_steps)
                    * count(z_steps, z)
                    * count(steps - z_steps, x, y);
        }
        return answer;
    }

private:
    int max_steps_;
    combination<mint> combination_;
};

}
