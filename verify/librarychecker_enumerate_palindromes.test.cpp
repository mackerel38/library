#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_palindromes"
#include <bits/stdc++.h>
#include "string/manacher.hpp"

using namespace std;
using namespace poe;

int main() {
    string text;
    cin >> text;
    const auto odd = manacher_odd(text);
    const auto even = manacher_even(text);
    for (int center = 0; center < static_cast<int>(text.size()); ++center) {
        if (center) cout << ' ' << 2 * even[center];
        cout << (center ? " " : "") << 2 * odd[center] - 1;
    }
    cout << '\n';
}
