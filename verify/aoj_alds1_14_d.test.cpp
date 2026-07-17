#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_14_D"
#include <bits/stdc++.h>
#include "string/suffixquery.hpp"

using namespace std;
using namespace poe;

int main() {
    string text;
    int queries;
    cin >> text >> queries;
    suffixquery suffix(text);
    while (queries--) {
        string pattern;
        cin >> pattern;
        cout << suffix.contains(pattern) << '\n';
    }
}
