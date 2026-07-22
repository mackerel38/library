#include <bits/stdc++.h>

#include "algorithm/modularrange.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    long long k;
    cin >> n >> k;
    vector<long long> a(n);
    for (auto& value : a) cin >> value;
    cout << minimum_modular_range(a, k) << '\n';
}
