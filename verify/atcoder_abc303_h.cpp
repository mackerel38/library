#define PROBLEM "https://atcoder.jp/contests/abc303/tasks/abc303_h"
#include <bits/stdc++.h>
#include "fps/combinatorial.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> allowed_degrees(k);
    for (int& degree : allowed_degrees) cin >> degree;
    cout << count_labeled_trees_with_degrees<998244353>(n, allowed_degrees) << '\n';
}
