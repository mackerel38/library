#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_n"
#include <bits/stdc++.h>
#include "fps/fps.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<int> count(n + 1);
    for (int value = 1; value <= n; ++value) cin >> count[value];
    fps998244353 logarithm(n + 1);
    for (int value = 1; value <= n; ++value) {
        for (int multiple = value, times = 1; multiple <= n; multiple += value, ++times) {
            logarithm[multiple] += modint998244353{1} / times;
        }
        const long long removed = 1LL * value * (count[value] + 1);
        for (long long multiple = removed, times = 1; multiple <= n; multiple += removed, ++times) {
            logarithm[static_cast<int>(multiple)] -= modint998244353{1} / times;
        }
    }
    cout << logarithm.exp(n + 1)[n] << '\n';
}
