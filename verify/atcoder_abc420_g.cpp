#define PROBLEM "https://atcoder.jp/contests/abc420/tasks/abc420_g"
#include <bits/stdc++.h>
#include "math/prime.hpp"

using namespace std;
using namespace poe;

int main() {
    long long x;
    cin >> x;
    const long long product = 4 * x - 1;
    set<long long> answers;
    for (const long long left : signed_divisors(product)) {
        const long long right = product / left;
        if ((left + right) % 4 != 0 || (left - right - 2) % 4 != 0) continue;
        const long long root = (left + right) / 4;
        const long long n = (left - right - 2) / 4;
        if (root >= 0) answers.insert(n);
    }
    cout << answers.size() << '\n';
    for (const long long answer : answers) cout << answer << ' ';
    cout << '\n';
}
