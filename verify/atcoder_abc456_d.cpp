#include <bits/stdc++.h>
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using mint = staticmodint<998244353>;
    string s;
    cin >> s;
    array<mint, 3> endings{};
    for (char c : s) {
        const int current = c - 'a';
        endings[current] = endings[0] + endings[1] + endings[2] + 1;
    }
    cout << (endings[0] + endings[1] + endings[2]).val() << '\n';
}
