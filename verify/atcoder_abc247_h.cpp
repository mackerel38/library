#include <bits/stdc++.h>
#include "fps/stirling.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long k;
    cin >> n >> k;
    vector<int> frequency(n + 1);
    for (int index = 0, color; index < n; ++index) {
        cin >> color;
        ++frequency[color];
    }
    vector<int> block_sizes;
    for (const int size : frequency) {
        if (size > 0) block_sizes.push_back(size);
    }
    const auto cycle_counts =
        block_permutation_cycle_counts<998244353>(block_sizes);
    modint998244353 answer = 0;
    for (int cycles = 0; cycles <= n; ++cycles) {
        const int minimum_swaps = n - cycles;
        if (minimum_swaps <= k && (k - minimum_swaps) % 2 == 0) {
            answer += cycle_counts[cycles];
        }
    }
    cout << answer << '\n';
}
