#include <bits/stdc++.h>

#include "string/fibonacciword.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string x, y;
    int q;
    cin >> x >> y >> q;
    fibonacciword word(x, y, 1000000000000000000ULL);
    while (q--) {
        unsigned long long left, right;
        char character;
        cin >> left >> right >> character;
        cout << word.count(character, left - 1, right) << '\n';
    }
}
