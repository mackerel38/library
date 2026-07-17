#include <bits/stdc++.h>
#include "string/runlength.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string text;
    cin >> text;
    for (auto [character, count] : runlength(text)) {
        cout << character << count;
    }
    cout << '\n';
}
