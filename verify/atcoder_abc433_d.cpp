#include <bits/stdc++.h>
#include "math/decimal.hpp"
using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    unsigned long long modulus;
    cin >> n >> modulus;
    vector<unsigned long long> values(n);
    for (auto& value : values) cin >> value;
    cout << count_divisible_concatenations(values, modulus) << '\n';
}
