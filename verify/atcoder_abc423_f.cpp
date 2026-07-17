#define PROBLEM "https://atcoder.jp/contests/abc423/tasks/abc423_f"
#include <bits/stdc++.h>
#include "math/subsettransform.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, required;
    long long limit;
    cin >> n >> required >> limit;
    vector<long long> divisors(n);
    for (auto& divisor : divisors) cin >> divisor;
    cout << divisibility_match_counts(limit, divisors)[required] << '\n';
}
