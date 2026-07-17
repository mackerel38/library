#define PROBLEM "https://atcoder.jp/contests/abc307/tasks/abc307_h"
#include <bits/stdc++.h>
#include "string/matching.hpp"

using namespace std;
using namespace poe;

int main() {
    int length, width;
    string text, pattern;
    cin >> length >> width >> text >> pattern;
    const string cycle = text + string(width - 1, '.');
    const string displays = cycle + cycle.substr(0, width - 1);
    cout << wildcard_matches(displays, pattern).size() << '\n';
}

