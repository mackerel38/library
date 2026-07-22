#pragma once
#include <bits/stdc++.h>
#include "math/combination.hpp"
#include "math/modint.hpp"
#include "math/prime.hpp"

namespace poe {

/// 構築O(p)、取得O(log_p n)。小さい素数pを法とする巨大な二項係数をLucasの定理で求める。
template <int prime>
struct lucascombination {
    using mint = staticmodint<prime>;

    /// O(p)時間・O(p)領域。0以上p未満の各桁に使う二項係数を前計算する。
    lucascombination() : digit_(prime - 1) {
        static_assert(prime >= 2);
        assert(is_prime(prime));
    }

    /// O(log_p n)。nCk mod pを返す。n<0、k<0、k>nでは0。
    mint choose(long long n, long long k) const {
        if (n < 0 || k < 0 || k > n) {
            return 0;
        }
        mint result = 1;
        while (n > 0 || k > 0) {
            const int n_digit = static_cast<int>(n % prime);
            const int k_digit = static_cast<int>(k % prime);
            if (k_digit > n_digit) {
                return 0;
            }
            result *= digit_.choose(n_digit, k_digit);
            n /= prime;
            k /= prime;
        }
        return result;
    }

private:
    combination<mint> digit_;
};

}
