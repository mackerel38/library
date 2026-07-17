#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "math/subsettransform.hpp"

using namespace std;
using namespace poe;

using mint = modint998244353;

struct decimal_block {
    mint value;
    mint power;

    void operator+=(const decimal_block& lower) {
        value = lower.value * power + value;
        power = lower.power * power;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int bit_count;
    string digits;
    cin >> bit_count >> digits;
    vector<decimal_block> value(digits.size());
    for (int mask = 0; mask < static_cast<int>(digits.size()); ++mask) {
        value[mask] = decimal_block{digits[mask] - '0', 10};
    }
    subset_zeta(value);
    long long answer = 0;
    for (int mask = 0; mask < static_cast<int>(value.size()); ++mask) {
        answer += value[mask].value.val() ^ mask;
    }
    cout << answer << '\n';
}
