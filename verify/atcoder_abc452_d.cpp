#include <bits/stdc++.h>

#include "string/subsequence.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s, t;
    cin >> s >> t;
    cout << count_substrings_without_subsequence(s, t) << '\n';
}
