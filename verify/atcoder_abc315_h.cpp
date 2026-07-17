#define PROBLEM "https://atcoder.jp/contests/abc315/tasks/abc315_h"
#include <bits/stdc++.h>
#include "fps/online.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    int n;
    cin >> n;
    vector<mint> multiplier(n + 1), answer(n + 1);
    for (int index = 1; index <= n; ++index) cin >> multiplier[index];
    answer[0] = 1;
    relaxedconvolution<998244353> square(n + 1);
    mint convolution_prefix = square.append(answer[0], answer[0]);
    for (int index = 1; index <= n; ++index) {
        answer[index] = multiplier[index] * convolution_prefix;
        convolution_prefix += square.append(answer[index], answer[index]);
    }
    for (int index = 1; index <= n; ++index) {
        if (index > 1) cout << ' ';
        cout << answer[index];
    }
    cout << '\n';
}
