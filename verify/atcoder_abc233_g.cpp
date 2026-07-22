#define PROBLEM "https://atcoder.jp/contests/abc233/tasks/abc233_g"

#include <bits/stdc++.h>

#include "dp/rectanglepartition.hpp"
#include "structure/prefixsum.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<vector<int>> marked(n, vector<int>(n));
    for (auto& row : marked) {
        string s;
        cin >> s;
        for (int column = 0; column < n; ++column) row[column] = s[column] == '#';
    }
    const prefixsum2d<int> sum(marked);
    cout << minimum_rectangle_partition_cost<int>(n, n,
        [&](int top, int left, int bottom, int right) {
            return sum.sum(top, left, bottom, right) == 0
                ? 0 : max(bottom - top, right - left);
        }) << '\n';
}
