#define PROBLEM "https://atcoder.jp/contests/abc141/tasks/abc141_f"
#include <bits/stdc++.h>
#include "math/xorbasis.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<unsigned long long> values(n);
    unsigned long long total = 0;
    for (auto& value : values) cin >> value, total ^= value;
    xorbasis<> basis;
    for (const auto value : values) basis.add(value & ~total);
    cout << total + 2 * basis.max() << '\n';
}
