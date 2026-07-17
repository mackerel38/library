#include <bits/stdc++.h>
#include "algorithm/isotonic.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n;
        cin >> n;
        vector<long long> values(n);
        for (int i = 0; i < n; ++i) {
            cin >> values[i];
            values[i] -= i;
        }
        cout << right_transfer_isotonic(values).operations << '\n';
    }
}
