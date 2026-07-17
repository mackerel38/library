#define PROBLEM "https://atcoder.jp/contests/abc318/tasks/abc318_h"
#include <bits/stdc++.h>
#include "fps/fps.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    int n;
    cin >> n;
    fps998244353 cycle(n + 1);
    mint factorial = 1;
    for (int size = 1; size <= n; ++size) {
        const mint inverse = mint{size}.inv();
        cycle[size] = inverse * inverse;
        factorial *= size;
    }
    const mint accepted_by_one = cycle.exp(n + 1)[n];
    cout << factorial * factorial * (1 - 2 * accepted_by_one) + factorial << '\n';
}
