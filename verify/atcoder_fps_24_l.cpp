#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_l"
#include <bits/stdc++.h>
#include "fps/fps.hpp"
#include "math/combination.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    combination<modint998244353> comb(n);
    fps998244353 cycles(n + 1);
    for (int size = 3; size <= n; ++size) cycles[size] = modint998244353{1} / size;
    cout << cycles.exp(n + 1)[n] * comb.factorial(n) << '\n';
}
