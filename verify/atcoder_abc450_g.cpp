#include <bits/stdc++.h>

#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<modint998244353> a(n);
    modint998244353 sum = 0, square_sum = 0;
    for (auto& value : a) {
        long long input;
        cin >> input;
        value = input;
        sum += value;
        square_sum += value * value;
    }
    if (n == 1) {
        cout << square_sum.val() << '\n';
        return 0;
    }
    modint998244353 coefficient = 0;
    for (int size = 2; size <= n; ++size)
        coefficient = -1 + modint998244353(size - 3) / (size - 1) * coefficient;
    const auto pair_sum = (sum * sum - square_sum) / 2;
    const auto pair_count = modint998244353(n) * (n - 1) / 2;
    cout << (square_sum + 2 * coefficient * pair_sum / pair_count).val() << '\n';
}
