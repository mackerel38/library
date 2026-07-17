#include <bits/stdc++.h>

#include "math/decimal.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int run_count;
    unsigned long long divisor;
    cin >> run_count >> divisor;
    vector<decimalrun> runs(run_count);
    for (auto& [digit, length] : runs) cin >> digit >> length;
    cout << decimal_runs_div_mod(runs, divisor, 10007) << '\n';
}
