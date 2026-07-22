#define PROBLEM "https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind"
#include <bits/stdc++.h>
#include "fps/stirling.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    const auto answer = stirling_second_row<998244353>(n);
    for (int k = 0; k <= n; ++k) {
        cout << answer[k] << (k == n ? '\n' : ' ');
    }
}
