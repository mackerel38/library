#pragma once
#include <bits/stdc++.h>

namespace poe {

/// recipe。O(n^2)。値を昇順に順列へ挿入し、隣接済み成分数ごとの並べ方数を返す。
/// allow(i,c,j): i個挿入済み・c成分から、新要素がj個の成分へ接続する遷移を許すか。
template<class T, class Allow>
std::vector<T> insertion_component_counts(int size, Allow allow) {
    assert(size >= 0);
    std::vector<T> dp(size + 1);
    dp[0] = T{1};
    for (int inserted = 0; inserted < size; ++inserted) {
        std::vector<T> next(size + 1);
        for (int components = 0; components <= inserted; ++components) {
            if (dp[components] == T{}) continue;
            for (int joined = 0; joined <= 2 && joined <= components; ++joined) {
                if (!std::invoke(allow, inserted, components, joined)) continue;
                const int next_components = components + 1 - joined;
                if (inserted + next_components > size) continue;
                int choices = 0;
                if (joined == 0) choices = components + 1;
                else if (joined == 1) choices = 2 * components;
                else choices = components - 1;
                next[next_components] += dp[components] * T{choices};
            }
        }
        dp.swap(next);
    }
    return dp;
}

}
