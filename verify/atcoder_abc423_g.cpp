#define PROBLEM "https://atcoder.jp/contests/abc423/tasks/abc423_g"
#include <bits/stdc++.h>
#include "math/decimal.hpp"

using namespace std;
using namespace poe;

int main() {
    int tests;
    cin >> tests;
    while (tests--) {
        int modulus;
        string pattern;
        cin >> modulus >> pattern;
        cout << smallest_multiple_containing(modulus, pattern) << '\n';
    }
}
