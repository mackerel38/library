#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_t"
#include <bits/stdc++.h>
#include "fps/rational.hpp"

using namespace std;
using namespace poe;

int main() {
    int colors;
    long long moves;
    cin >> colors >> moves;
    vector<modint998244353> count(colors);
    for (auto& value : count) cin >> value;

    using series = fps998244353;
    vector<rationalfps<998244353>> terms;
    for (int color = 1; color < colors; ++color) {
        terms.push_back({series{0, count[color]}, series{1, count[color]}});
    }
    auto blocks = rational_sum<998244353>(std::move(terms));
    series denominator = blocks.denominator - blocks.numerator;
    series delayed = blocks.numerator;
    delayed.insert(delayed.begin(), 0);
    denominator -= delayed * count[0];
    cout << rationalfps<998244353>{blocks.numerator, denominator}.coeff(moves - 1) << '\n';
}
