#define PROBLEM "https://atcoder.jp/contests/abc424/tasks/abc424_f"
#include <bits/stdc++.h>
#include "structure/laminarintervalset.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    laminarintervalset chords(n);
    while (q--) {
        int left, right;
        cin >> left >> right;
        cout << (chords.add(left - 1, right - 1) ? "Yes" : "No") << '\n';
    }
}
