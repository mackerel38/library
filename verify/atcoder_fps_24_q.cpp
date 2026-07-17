#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_q"
#include <bits/stdc++.h>
#include "fps/rational.hpp"
#include "fps/transform.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m, maximum_power;
    cin >> n >> m >> maximum_power;
    vector<modint998244353> a(n), b(m);
    for (auto& value : a) cin >> value;
    for (auto& value : b) cin >> value;
    auto left = power_sums<998244353>(a, maximum_power + 1);
    auto right = power_sums<998244353>(b, maximum_power + 1);
    auto sums = binomial_convolution<998244353>(left, right).prefix(maximum_power + 1);
    const modint998244353 denominator = modint998244353{n} * m;
    for (int power = 1; power <= maximum_power; ++power) cout << sums[power] / denominator << '\n';
}
