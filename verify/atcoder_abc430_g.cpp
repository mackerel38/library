#define PROBLEM "https://atcoder.jp/contests/abc430/tasks/abc430_g"
#include <bits/stdc++.h>
#include "recipe/setsegtree.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, queries;
    cin >> n >> queries;
    setsegtree tree(n);
    while (queries--) {
        int type, left, right;
        cin >> type >> left >> right;
        --left;
        if (type == 3) {
            const auto [maximum, count] = tree.max(left, right);
            cout << maximum << ' ' << count << '\n';
        } else {
            int element;
            cin >> element;
            --element;
            if (type == 1) tree.add(left, right, element);
            else tree.erase(left, right, element);
        }
    }
}
