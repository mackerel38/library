#define PROBLEM "https://atcoder.jp/contests/abc408/tasks/abc408_g"
#include <bits/stdc++.h>
#include "math/fraction.hpp"

using namespace std;
using namespace poe;

int main() {
    int test_count;
    cin >> test_count;
    while (test_count--) {
        long long a, b, c, d;
        cin >> a >> b >> c >> d;
        cout << simplest_fraction_between(a, b, c, d).denominator << '\n';
    }
}
