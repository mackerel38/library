#define PROBLEM "https://atcoder.jp/contests/abc425/tasks/abc425_e"
#include <bits/stdc++.h>
#include "math/combination.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests, modulus;
    cin >> tests >> modulus;
    pascalcombination combination(5000, modulus);
    while (tests--) {
        int n;
        cin >> n;
        vector<int> counts(n);
        for (int& count : counts) cin >> count;
        cout << combination.multinomial(counts) << '\n';
    }
}
