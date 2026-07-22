#include <bits/stdc++.h>
#include "recipe/compassmove.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        long long x, y;
        string allowed;
        cin >> x >> y >> allowed;
        cout << compassmoves(allowed).distance(x, y).value_or(-1) << '\n';
    }
}
