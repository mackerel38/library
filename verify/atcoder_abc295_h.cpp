#define PROBLEM "https://atcoder.jp/contests/abc295/tasks/abc295_h"
#include <bits/stdc++.h>
#include "recipe/prefixgrid.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m;
    cin >> n >> m;
    vector<string> pattern(n);
    for (string& row : pattern) cin >> row;
    cout << count_row_column_prefix_union<998244353>(pattern) << '\n';
}
