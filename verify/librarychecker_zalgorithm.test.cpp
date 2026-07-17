#define PROBLEM "https://judge.yosupo.jp/problem/zalgorithm"
#include <bits/stdc++.h>
#include "string/zalgorithm.hpp"

using namespace std;
using namespace poe;

int main() {
    string text;
    cin >> text;
    const auto z = z_algorithm(text);
    for (int i = 0; i < static_cast<int>(z.size()); ++i) {
        if (i) cout << ' ';
        cout << z[i];
    }
    cout << '\n';
}
