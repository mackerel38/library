#define PROBLEM "https://judge.yosupo.jp/problem/number_of_substrings"
#include <bits/stdc++.h>
#include "string/lcp.hpp"
#include "string/suffixarray.hpp"

using namespace std;
using namespace poe;

int main() {
    string text;
    cin >> text;
    const auto suffixes = suffix_array(text);
    const auto lcp = lcp_array(text, suffixes);
    long long answer = 1LL * text.size() * (text.size() + 1) / 2;
    answer -= reduce(lcp.begin(), lcp.end(), 0LL);
    cout << answer << '\n';
}
