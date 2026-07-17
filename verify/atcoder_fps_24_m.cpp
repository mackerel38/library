#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_m"
#include <bits/stdc++.h>
#include "fps/fps.hpp"
#include "math/combination.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    combination<modint998244353> comb(n);
    fps998244353 graph(n + 1);
    for (int size = 0; size <= n; ++size) {
        graph[size] = modint998244353{2}.pow(1LL * size * (size - 1) / 2) / comb.factorial(size);
    }
    cout << graph.log(n + 1)[n] * comb.factorial(n) << '\n';
}
