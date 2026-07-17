#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_o"
#include <bits/stdc++.h>
#include "fps/fps.hpp"
#include "math/combination.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<bool> prime(n + 1, true);
    prime[0] = prime[1] = false;
    for (int p = 2; p * p <= n; ++p) if (prime[p]) {
        for (int multiple = p * p; multiple <= n; multiple += p) prime[multiple] = false;
    }
    combination<modint998244353> comb(n);
    fps998244353 choices(n);
    choices[0] = 1;
    for (int children = 2; children < n; ++children) {
        if (prime[children]) choices[children] = modint998244353{1} / comb.factorial(children);
    }
    cout << choices.pow(n, n)[n - 1] * comb.factorial(n - 1) / n << '\n';
}
