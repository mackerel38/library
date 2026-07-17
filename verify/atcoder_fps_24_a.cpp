#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_a"
#include <bits/stdc++.h>
#include "fps/sparse.hpp"

using namespace std;
using namespace poe;

int main() {
    int days, total;
    cin >> days >> total;
    fps998244353 one_day(total + 1);
    for (int price : {1, 3, 4, 6}) if (price <= total) one_day[price] = 1;
    cout << sparse_power<998244353>(one_day, days, total + 1)[total] << '\n';
}
