#define PROBLEM "https://atcoder.jp/contests/abc428/tasks/abc428_c"
#include <bits/stdc++.h>
#include "string/bracketsequence.hpp"

using namespace std;
using namespace poe;

int main() {
    int q;
    cin >> q;
    bracketsequence brackets;
    brackets.reserve(q);
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            char value;
            cin >> value;
            brackets.push(value);
        } else {
            brackets.pop();
        }
        cout << (brackets.good() ? "Yes" : "No") << '\n';
    }
}
