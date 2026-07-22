#include <bits/stdc++.h>
#include "string/avoid.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string text;
    int pattern_count;
    cin >> text >> pattern_count;
    vector<string> patterns(pattern_count);
    for (auto& pattern : patterns) cin >> pattern;
    cout << minimum_pattern_replacements(text, patterns).size() << '\n';
}
