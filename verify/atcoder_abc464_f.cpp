#include <bits/stdc++.h>
#include "dp/subsetsum.hpp"
#include "math/combination.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

modint998244353 convert(__int128_t value) {
    constexpr long long mod = 998244353;
    long long result = static_cast<long long>(value % mod);
    if (result < 0) result += mod;
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long threshold;
    cin >> n >> threshold;
    vector<long long> values(n);
    long long total = 0;
    for (long long& value : values) {
        cin >> value;
        total += value;
    }
    const auto statistics = subset_sum_less_by_size(values, threshold);
    combination<modint998244353> combinations(n);
    modint998244353 answer = 0;
    for (int size = 0; size < n; ++size) {
        const __int128_t numerator =
            __int128_t{statistics.count[size]} * total - statistics.sum[size];
        answer += convert(numerator) /
            (combinations.choose(n, size) * modint998244353(n - size));
    }
    cout << answer << '\n';
}
