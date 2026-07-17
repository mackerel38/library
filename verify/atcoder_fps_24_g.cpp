#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_g"
#include <bits/stdc++.h>
#include "fps/sparse.hpp"

using namespace std;
using namespace poe;

int main() {
    int total, maximum, width;
    cin >> total >> maximum >> width;
    fps998244353 ways(total + 1);
    ways[0] = 1;
    for (int coin = 1; coin <= width; ++coin) {
        ways = divide_binomial(std::move(ways), coin, modint998244353{-1}, total + 1);
    }
    for (int left = 1; left + width - 1 <= maximum; ++left) {
        cout << ways[total] << '\n';
        ways = multiply_binomial(std::move(ways), left, modint998244353{-1}, total + 1);
        if (left + width <= maximum) {
            ways = divide_binomial(std::move(ways), left + width, modint998244353{-1}, total + 1);
        }
    }
}
