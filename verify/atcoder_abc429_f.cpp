#define PROBLEM "https://atcoder.jp/contests/abc429/tasks/abc429_f"
#include <bits/stdc++.h>
#include "recipe/monotonegridshortestpath.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<string> cells(3);
    for (auto& row : cells) cin >> row;
    monotonegridshortestpath<3> path(cells);
    int q;
    cin >> q;
    while (q--) {
        int row, column;
        cin >> row >> column;
        path.toggle(row - 1, column - 1);
        cout << path.distance(0, 0, 2, n - 1) << '\n';
    }
}
