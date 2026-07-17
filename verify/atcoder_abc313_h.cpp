#define PROBLEM "https://atcoder.jp/contests/abc313/tasks/abc313_h"
#include <bits/stdc++.h>
#include "dp/permutation.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    int n;
    cin >> n;
    vector<long long> front(n), back(n + 1);
    for (long long& height : front) cin >> height;
    for (long long& height : back) cin >> height;
    ranges::sort(front);
    ranges::sort(back);
    const auto counts = insertion_component_counts<mint>(
        n,
        [&](int inserted, int components, int joined) {
            if (joined <= 1 && back[inserted + components] < front[inserted]) {
                return false;
            }
            return true;
        }
    );
    cout << counts[1] << '\n';
}
