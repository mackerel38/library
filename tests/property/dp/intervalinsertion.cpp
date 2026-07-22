#include <bits/stdc++.h>
#include "dp/intervalinsertion.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;
using mint = modint998244353;

mint brute(string_view directions) {
    const int n = directions.size();
    vector<optional<mint>> memo(1 << n);
    auto solve = [&](auto&& self, int mask) -> mint {
        if (popcount(static_cast<unsigned>(mask)) == 1) return 0;
        if (memo[mask]) return *memo[mask];
        mint result = 0;
        const int people = popcount(static_cast<unsigned>(mask));
        for (int chosen = 0; chosen < n; ++chosen) {
            if (!(mask >> chosen & 1)) continue;
            const int step = directions[chosen] == 'R' ? 1 : -1;
            int removed = chosen;
            int distance = 0;
            do {
                removed = (removed + step + n) % n;
                ++distance;
            } while (!(mask >> removed & 1));
            result += mint{distance} + self(self, mask ^ (1 << removed));
        }
        memo[mask] = result / people;
        return *memo[mask];
    };
    return solve(solve, (1 << n) - 1);
}

int main() {
    for (int n = 2; n <= 9; ++n) {
        for (int mask = 0; mask < (1 << n); ++mask) {
            string directions(n, 'L');
            for (int index = 0; index < n; ++index) {
                if (mask >> index & 1) directions[index] = 'R';
            }
            assert(circular_directional_removal_expectation<mint>(directions)
                   == brute(directions));
        }
    }
}
