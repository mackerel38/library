#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_b"
#include <bits/stdc++.h>
#include "fps/rational.hpp"

using namespace std;
using namespace poe;

int main() {
    long long n;
    cin >> n;
    cout << count_weighted_sum<998244353>(n, {{1, 1}, {1, 2}, {2}, {3}}) << '\n';
}
