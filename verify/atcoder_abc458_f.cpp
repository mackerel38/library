#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "string/avoid.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long length;
    int pattern_count;
    cin >> length >> pattern_count;
    vector<string> patterns(pattern_count);
    for (auto& pattern : patterns) cin >> pattern;
    cout << count_avoiding_words<modint998244353>(length, patterns, 'a', 'z') << '\n';
}
