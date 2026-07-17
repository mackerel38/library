#define PROBLEM "https://atcoder.jp/contests/abc300/tasks/abc300_h"
#include <bits/stdc++.h>
#include "fps/rational.hpp"

using namespace std;
using namespace poe;

int main() {
    int k;
    unsigned long long n;
    cin >> k >> n;
    fps998244353 denominator(k + 1, -1);
    denominator[0] = 1;
    fps998244353 numerator = denominator * fps998244353(k, 1);
    numerator.resize(k);
    cout << rationalfps<998244353>{numerator, denominator}.submask_sum(n) << '\n';
}
