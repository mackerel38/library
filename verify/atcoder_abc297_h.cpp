#define PROBLEM "https://atcoder.jp/contests/abc297/tasks/abc297_h"
#include <bits/stdc++.h>
#include "fps/combinatorial.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<int> weights(n);
    iota(weights.begin(), weights.end(), 1);
    cout << adjacent_distinct_words<998244353>(n, weights).length_sum[n] << '\n';
}
