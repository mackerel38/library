#include <bits/stdc++.h>
#include "structure/waveletmatrix.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> values(n);
    for (int& value : values) cin >> value;
    waveletmatrix<int> wm(values);

    int q;
    cin >> q;
    while (q--) {
        int left, right, value;
        cin >> left >> right >> value;
        cout << wm.count(left - 1, right, value) << '\n';
    }
}
