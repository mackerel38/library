#define PROBLEM "https://atcoder.jp/contests/abc232/tasks/abc232_g"

#include <bits/stdc++.h>

#include "graph/modularcomplete.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long modulus;
    cin >> n >> modulus;
    vector<long long> outgoing(n), incoming(n);
    for (auto& value : outgoing) cin >> value;
    for (auto& value : incoming) cin >> value;
    cout << modular_complete_distances(outgoing, incoming, modulus)[n - 1] << '\n';
}
