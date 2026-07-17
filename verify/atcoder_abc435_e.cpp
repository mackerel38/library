#include <bits/stdc++.h>
#include "structure/intervalset.hpp"
using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n;
    int q;
    cin >> n >> q;
    intervalset<long long> white;
    white.add(0, n);
    while (q--) {
        long long left, right;
        cin >> left >> right;
        white.erase(left - 1, right);
        cout << white.covered() << '\n';
    }
}
