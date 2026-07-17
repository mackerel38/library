#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_k"
#include <bits/stdc++.h>
#include "fps/fps.hpp"
#include "math/combination.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    combination<modint998244353> comb(n);
    fps998244353 all(n + 1);
    for (int size = 0; size <= n; ++size) all[size] = comb.factorial(size);
    cout << -all.inv(n + 1)[n] << '\n';
}
