#include <bits/stdc++.h>
#include "algorithm/quadraticselection.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<long long> cost(n);
    for (auto& value : cost) cin >> value;

    vector<pair<long long, long long>> points(n);
    for (int index = 0; index < n; ++index) {
        long long sum = 0, square_sum = 0;
        for (int face = 0; face < 6; ++face) {
            long long value;
            cin >> value;
            sum += value;
            square_sum += value * value;
        }
        points[index] = {sum, 6 * square_sum - sum * sum - 36 * cost[index]};
    }

    const __int128_t scaled = maximum_quadratic_k_selection(points, k);
    long long remainder = static_cast<long long>(scaled % modint998244353::mod());
    if (remainder < 0) remainder += modint998244353::mod();
    cout << modint998244353(remainder) / 36 << '\n';
}

