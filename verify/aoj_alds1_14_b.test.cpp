#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_14_B"
#include <bits/stdc++.h>
#include "string/prefix.hpp"

using namespace std;
using namespace poe;

int main() {
    string text, pattern;
    cin >> text >> pattern;
    for (const int position : find_all(text, pattern)) cout << position << '\n';
}
