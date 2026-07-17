#define PROBLEM "https://atcoder.jp/contests/abc236/tasks/abc236_f"
#include <bits/stdc++.h>
#include "math/xorbasis.hpp"

using namespace std;
using namespace poe;

int main() {
    int bits;
    cin >> bits;
    vector<pair<long long, unsigned long long>> spices;
    for (int value = 1; value < (1 << bits); ++value) {
        long long cost;
        cin >> cost;
        spices.emplace_back(cost, value);
    }
    ranges::sort(spices);
    xorbasis<> basis;
    long long answer = 0;
    for (const auto [cost, value] : spices) if (basis.add(value)) answer += cost;
    cout << answer << '\n';
}
