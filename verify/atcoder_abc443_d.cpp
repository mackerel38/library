#include <bits/stdc++.h>
#include "algorithm/slopeconstraint.hpp"

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
        vector<long long> row(n);
        long long answer = 0;
        for (long long& value : row) cin >> value, answer += value;
        for (long long value : slope_minorant(row, 1LL)) answer -= value;
        cout << answer << '\n';
    }
}
