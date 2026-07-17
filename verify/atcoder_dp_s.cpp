#define PROBLEM "https://atcoder.jp/contests/dp/tasks/dp_s"
#include <bits/stdc++.h>
#include "dp/digit.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    string upper;
    int divisor;
    cin >> upper >> divisor;
    using mint = modint1000000007;
    const auto result = digitdp<mint>(
        upper, 0,
        [&](int remainder, int digit, int) { return (remainder + digit) % divisor; },
        leadingzero::use
    );
    cout << (result.sum([](int remainder, bool) { return remainder == 0; }) - 1).val() << '\n';
}
