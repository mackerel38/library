#define PROBLEM "https://atcoder.jp/contests/abc425/tasks/abc425_f"
#include <bits/stdc++.h>
#include "dp/subset.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string target;
    cin >> n >> target;
    cout << count_distinct_deletion_orders<modint998244353>(target).val() << '\n';
}
