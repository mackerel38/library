#define PROBLEM "https://atcoder.jp/contests/abc279/tasks/abc279_h"
#include <bits/stdc++.h>
#include "fps/partition.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, m;
    cin >> n >> m;
    cout << euler_product_over_one_minus_x_coefficient<200003>(m - n, 2 * n)
         << '\n';
}
