#define PROBLEM "https://atcoder.jp/contests/abc425/tasks/abc425_g"
#include <bits/stdc++.h>
#include "structure/xortrie.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    unsigned long long upper;
    cin >> n >> upper;
    vector<unsigned> values(n);
    for (auto& value : values) cin >> value;
    cout << sum_min_xor<unsigned, 30>(values, upper) << '\n';
}
