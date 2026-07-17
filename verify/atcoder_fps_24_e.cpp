#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_e"
#include <bits/stdc++.h>
#include "fps/polynomial.hpp"
#include "math/combination.hpp"

using namespace std;
using namespace poe;

int main() {
    int length, maximum;
    cin >> length >> maximum;
    combination<modint998244353> comb(length);
    vector<fps998244353> factors;
    for (int value = 1; value <= maximum; ++value) {
        fps998244353 factor(min(value, length) + 1);
        for (int count = 0; count < factor.size_int(); ++count) {
            factor[count] = comb.choose(length, count) / comb.permutation(length, count);
        }
        factors.push_back(std::move(factor));
    }
    auto product = polynomial_product<998244353>(std::move(factors), length + 1);
    cout << product[length] * comb.factorial(length) << '\n';
}
