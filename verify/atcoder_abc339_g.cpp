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
    waveletmatrix<int, long long> wm(values);

    int q;
    cin >> q;
    long long answer = 0;
    while (q--) {
        long long encoded_left, encoded_right, encoded_upper;
        cin >> encoded_left >> encoded_right >> encoded_upper;
        const int left = static_cast<int>(encoded_left ^ answer);
        const int right = static_cast<int>(encoded_right ^ answer);
        const int upper = static_cast<int>(encoded_upper ^ answer);
        answer = wm.sum_leq(left - 1, right, upper);
        cout << answer << '\n';
    }
}
