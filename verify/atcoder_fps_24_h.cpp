#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_h"
#include <bits/stdc++.h>
#include "fps/transform.hpp"
#include "math/combination.hpp"

using namespace std;
using namespace poe;

int main() {
    int x, y;
    cin >> x >> y;
    const int operations = x + y;
    combination<modint998244353> comb(2 * operations);
    fps998244353 with_zero(operations + 1);
    for (int count = 1; count <= operations; ++count) {
        with_zero[count] = comb.choose(count, x) * comb.choose(y + count - 1, y);
    }
    const auto without_zero = binomial_transform<998244353>(with_zero, modint998244353{-1});
    cout << reduce(without_zero.begin(), without_zero.end(), modint998244353{}) << '\n';
}
